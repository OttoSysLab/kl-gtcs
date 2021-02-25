#include <math.h>
#include <qvaluelist.h>
#include <qstringlist.h>

#include <qtopia/businesslogic/globaldefine.h>
#include <qtopia/dspinterface/dspinterface.h>
#include <qtopia/qcopenvelope_qws.h>
#include <qtopia/businesslogic/board.h>
#include <qtopia/businesslogic/autoworkstationrequest.h>

#include "toolformfgtune.h"
#include "antennatunetable.h"
#include "dspcontrol.h"
#include "mfgtunemode.h"
#include "receiveconditionswitch.h"
#include "calibratedata.h"
#include "dspregisterdebug.h"
#include <qtopia/alertmode/ringmodehandler.h>
#include <unistd.h>
#include <qfile.h>
#include <qdir.h>
#include <qtextstream.h>
#include <qtopia/audioV2/audioPackerV2.h>
#include <qtopia/config.h>

#define Squelch_Config "/runpara/Applications/mfg_tune/Squelch.conf"
#define MfgDiableConfig "/runpara/Applications/mfg_tune/MFG_Disable.csv"
#define Lvl (-60)
#define Analog 	(1)
#define Digital (2)
#define REFRESHTIME (1)

#define MFG_Squelch    (0)
#define MFG_BER        (1)
#define Golden_Squelch (2)
#define Golden_BER     (3)

TMFGTuneMode * TMFGTuneMode::m_instance = NULL;

TMFGTuneMode * TMFGTuneMode::instance()
{
    if (m_instance == NULL) {
        m_instance = new TMFGTuneMode();
    }

    return m_instance;
}

/**
 *
 *  @author  Alex
 *
 *  @date    2016/06/20
 *
 *  @fn      TMFGTuneMode::TMFGTuneMode(QObject *parent)
 *
 *  @brief   ( constructor )
 *
 *  @param
 *
 *  @return  none
 *
 *  @note    none
 *
 */
TMFGTuneMode::TMFGTuneMode(QObject *parent)
:QObject(parent)
{
    u3log_debug("TMFGTuneMode::TMFGTuneMode");
    
    TDSPRegisterDebug::instance();

    m_AntennaTuneTable = new TAntennaTuneTable(this);
    m_DSPControl = new TDSPControl(this);

    //Construction TReceiveconditionswitch class    
    m_OtaCHswitch = new TReceiveconditionswitch(this);  

    // write AntennaTuneInfoList data to calibration data
    m_TCalibrateData = new TCalibrateData(this);

    m_13MHzDAC = 0;
    m_13MhzStartDAC = 0;

    // receive qcop signal
    QCopChannel *ch = new QCopChannel("G4/MFGTune", this);
    connect(ch, SIGNAL(received(const QCString&, const QByteArray&)), 
        this, SLOT(slt_msgReceived(const QCString &, const QByteArray &)));
	
    // receive 13MHz Tune result
    connect(m_DSPControl, SIGNAL(sig_13MHzTuneStop(unsigned short)), this, SLOT(slt_13MHzResult(unsigned short)));
	
    // receive Squelch Timeout
    connect(m_DSPControl, SIGNAL(sig_SquelchTuneStop()), this, SLOT(slt_SquelchTimeout()));
	
	//Delay 
	m_DelayTimer= new QTimer(this);
    connect(m_DelayTimer, SIGNAL(timeout()), this, SLOT(slt_delayTimeout()));
	
	//passiver param
	QCopChannel *ch1 = new QCopChannel("QPE/TransmitUIReq", this);
    connect(ch1, SIGNAL(received(const QCString&, const QByteArray&)),
            this, SLOT(slt_msgReceived(const QCString &, const QByteArray &)));
	
	// receive autoworstation qcop signal
    QCopChannel *ch2 = new QCopChannel("QPE/BerRssi", this);
    connect(ch2, SIGNAL(received(const QCString&, const QByteArray&)), 
        this, SLOT(slt_getBerRssi(const QCString &, const QByteArray &)));
	
	//for test
	QCopChannel *test = new QCopChannel("G4/MFGTune", this);
	connect(test, SIGNAL(received(const QCString&, const QByteArray&)), 
        this, SLOT(slt_test(const QCString &, const QByteArray &)));
        
	m_TimerState = Idle;
	m_testMode = false;
}

/**
 *
 *  @author  Alex
 *
 *  @date    2016/06/20
 *
 *  @fn      TMFGTuneMode::~TMFGTuneMode()
 *
 *  @brief   ( destructor )
 *
 *  @param
 *
 *  @return  none
 *
 *  @note    none
 *
 */
TMFGTuneMode::~TMFGTuneMode()
{
    
}

/**
 *
 *  @author  Alex
 *
 *  @date    2016/06/20
 *
 *  @fn      void TMFGTuneMode::slt_msgReceived(const QCString& msg, const QByteArray& data)
 *
 *  @brief   ( receive ota order & test result order )
 *
 *  @param   ...
 *
 *  @return  none
 *
 *  @note    none
 *
 */
void TMFGTuneMode::slt_msgReceived(const QCString& msg, const QByteArray& data)
{
    u3log_debug("== TMFGTuneMode::slt_msgReceived ==[%s]",msg.data());

    // Test 13MHz Tune
    if (msg == "autoDAC()")
    {
        u3log_error("======== autoDAC() ========");

        // Set to General Mode
        TBoard::setIsMFGTuneMode(true);
        // set value to starting DSPControl
        m_DSPControl->Start13MHzTuneSet(100);
    }
    else if (!TBoard::isMFGTuneMode())
    {
        u3log_warn("This is not MFG Tune Mode");

        return;
    }

    // MFGTune Stop
    if (msg == "MFGTuneModeStop()")
    {
        u3log_debug("======== MFGTuneModeStop() ========");

        //Back To before Zone
        m_OtaCHswitch->ToBackZone();

		//notify UI
		QCopEnvelope e("G4/MFGTune", "UIMFGTuneModeStop()");
    }
    // 13MHz Tune Start
    else if(msg == "T13MHzTuneStart()")
    {
        u3log_debug("======== T13MHzTuneStart() ========");
    }
    // 13MHz Tune Set
    else if(msg == "T13MHzFreqTune(QByteArray)")
    {
        u3log_debug("======== T13MHzFreqTune(QByteArray) ========");

        Q_UINT32 idCode;
        Q_UINT32 stableTime;
        Q_UINT32 upperLimit;
        Q_UINT32 lowerLimit;
        Q_UINT32 frequency;
        Q_UINT32 protocol;
        Q_UINT32 subAudio;
        Q_UINT32 groupID;

        QDataStream stream(data,IO_ReadOnly);
        // get ota 13MHz tune value
        stream >> idCode >> stableTime >> m_13MHzUpperLimitDAC >> m_13MHzLowerLimitDAC >> frequency >> protocol >> subAudio >> groupID;

        u3log_debug("parse stableTime:%u,upperLimit:%u,lowerLimit:%u,frequency:%u,protocol:%u,subAudio:%u,groupID:%u ",
            stableTime,m_13MHzUpperLimitDAC,m_13MHzLowerLimitDAC,frequency,protocol,subAudio,groupID);

        // set value to starting DSPControl
        m_13MhzStartDAC = m_DSPControl->Start13MHzTuneSet(stableTime);

        //13MHz Tune start G4 in Repeater Mode set frequency
        m_OtaCHswitch->SetOTA13MHzMode(frequency);
    }
    // 13MHz Tune Stop 
    else if(msg == "T13MHzTuneStop()")
    {
        u3log_debug("======== T13MHzTuneStop() ========");

        // save to file
        ToolForMfg::Write13MHzTuneValueToFile(m_13MHzDAC);

        if (m_13MHzDAC > m_13MHzUpperLimitDAC || m_13MHzDAC < m_13MHzLowerLimitDAC)
        {
            // Show 13Mhz Error Red
            QCopEnvelope e("G4/MFGTune", "T13MHzTuneStopError()");
            e << 1;

            // Stay on Test Mode
            m_OtaCHswitch->SetdisableRadio();
        }
        else if (m_13MhzStartDAC == m_13MHzDAC)
        {
            // Show 13Mhz Error Red
            QCopEnvelope e("G4/MFGTune", "T13MHzTuneStopError()");
            e << 2;

            // Stay on Test Mode
            m_OtaCHswitch->SetdisableRadio();
        }
        else
        {
            // Write to Calibration
            m_TCalibrateData->Write13MHzResult(m_13MHzDAC);
        }
    }
    // AntennaTune Start
    else if (msg == "AntennaTuneStart()")
    {
        u3log_debug("======== AntennaTuneStart() ========");

        MFGControlChFreqInfo tmp_mfgControlChInfo;
        //read Applications/mfg_tune/mfg.db data to tmp_mfgControlChInfo 
        TBusinessGlobal::getMFGControlChInfo( tmp_mfgControlChInfo );
        m_DACTuneCount = tmp_mfgControlChInfo.DACTuneCount;

        m_AntennaTuneTable->InitTable(m_DACTuneCount);
    }
    // AntennaTune Set
    else if(msg == "AntennaTune(QByteArray)")
    {
        u3log_debug("======== AntennaTune(QByteArray) ========");

        Q_UINT32 idCode;
        Q_UINT32 rssiSamplingRate;
        Q_UINT8 frequencyNo;
        Q_UINT16 startDAC;
        Q_UINT16 stopDAC;
        Q_UINT16 stepDAC;
        Q_UINT32 frequency;
        float rssiLevel;

        QDataStream stream(data,IO_ReadOnly);
        // get ota antenna tune value
        stream >> idCode >> rssiSamplingRate >> frequencyNo >> startDAC >> stopDAC >> stepDAC >> frequency >> rssiLevel;

        u3log_debug("parse rssiSamplingRate:%u,frequencyNo:%u,startDAC:%u,stopDAC:%u,stepDAC:%u,frequency:%u,rssiLevel:%f",
            rssiSamplingRate,frequencyNo,startDAC,stopDAC,stepDAC,frequency,rssiLevel);

        // set rssi level
        m_AntennaTuneTable->SetRSSILevel(frequencyNo, rssiLevel);

        // record value
        m_Condition.item = frequencyNo;
        m_Condition.startDAC = startDAC;
        m_Condition.stopDAC = stopDAC;
        m_Condition.stepDAC = stepDAC;
        m_Condition.freq = frequency;
        m_Condition.sampleRate = rssiSamplingRate;

        // set value to starting DSPControl
        m_DSPControl->StartAntennaTuneSet(m_Condition);
    }
    // AntennaTune End
    else if (msg == "AntennaTuneStop()")
    {
        u3log_debug("======== AntennaTuneStop() ========");

        // Set Interpolation
        m_AntennaTuneTable->Interpolation();

        AntennaTuneInfoList list = m_AntennaTuneTable->GetTable();

        // save to file
        ToolForMfg::WriteAntennaTuneValueToFile(list);

        // Write Calibration
        m_TCalibrateData->WriteCalibrateData(list);

        // print table
        m_AntennaTuneTable->printTable();

         // Check Table
        int checkno = 0;
        if (false == m_AntennaTuneTable->CheckTable(checkno))
        {
            // Show Antenna Tune Stop Error
            QCopEnvelope e("G4/MFGTune", "AntennaTuneStopError(int)");
            e << checkno;
            for (int i = 0; i < m_DACTuneCount; i++)
            {
                if (list[i].condition)
                {
                    e << list[i].item << list[i].DAC << list[i].rssi;
                }
            }

            // Stay on Test Mode
            m_OtaCHswitch->SetdisableRadio();
        }
    }
    else if(msg == "RssiResult(QByteArray)")
    {
        u3log_debug("======== RssiResult(QByteArray) ========");

        int item;
        int dac;
        unsigned int sm, smf;
        float rssi;
        QDataStream stream(data,IO_ReadOnly);
        // get rssi value
        stream >> sm >> smf;

        // math rssi
        rssi = 10*log10((sm+(smf/(pow(2,32))))/2147483648);
        u3log_debug("rssi<%f>", rssi);

        // set value to DSPControl
        // if test end in this item?
        if (m_DSPControl->NextAntennaTuneSet(item, dac, rssi))
        {
            u3log_debug("better dac<%d> rssi<%f>", dac, rssi);
        }
        else
        {
            u3log_debug("check item<%d> over, best dac<%d> rssi<%f>", item, dac, rssi);

            m_AntennaTuneTable->SetValue(item, dac, rssi);
        }
    }
    // SquelchTune Start
    else if (msg == "SquelchTuneStart()")
    {
        u3log_debug("======== SquelchTuneStart() ========");
        m_OtaCHswitch->InitSquelchTuneData();
    }
    // SquelchTune(QByteArray)
    else if (msg == "SquelchTune(QByteArray)")
    {
        u3log_debug("======== SquelchTune(QByteArray) ========");

        QDataStream stream(data,IO_ReadOnly);
        // get ota squelch tune value
        stream >> m_param.idCode>> m_param.SQType >> m_param.SQLevel >> m_param.SQPassLevel >> m_param.SQTimeout >> m_param.SQAudioPath >> m_param.SQFreq >> m_param.SQProtocol;
		u3log_debug("idCode[%d],SQType[%d],SQLevel[%d],SQPassLevel[%f],SQTimeout[%d]",m_param.idCode,m_param.SQType,m_param.SQLevel,m_param.SQPassLevel,m_param.SQTimeout);
		u3log_debug("SQAudioPath[%d],SQFreq[%d],SQProtocol[%d]",m_param.SQAudioPath,m_param.SQFreq,m_param.SQProtocol);

        // Squelch Test Set
        m_OtaCHswitch->SetSquelchTune(m_param.SQFreq,m_param.SQProtocol);

        if ( 0 != m_param.SQAudioPath )
        {
            // Set Speaker to External
            TShareMemory().m_shm->spkDev[0] = SpkAudDev_External;
        }
    
		if(Golden_Squelch == m_param.SQType)
		{
			//set a timer to delay ,and then start getting RSSI
			Config config(Squelch_Config,Config::File);
			config.setGroup("GoldenSquelch");
			int dTime = config.readNumEntry("DelayTimer",3);
			m_DelayTimer->start(dTime*1000,true);
			u3log_debug("DelayTime[%d]",dTime*1000);
    }
    }
    // SquelchTune Stop
    else if (msg == "SquelchTuneStop()")
    {
        u3log_debug("======== SquelchTuneStop() ========");
		QCopEnvelope e("G4/MFGTune", "SquelchTuneResult(int)");
		e << (Q_UINT32)m_param.SQType;
    }
	else if (msg == "recv()")
    {
        u3log_debug("msg == TRANSMITUIREQ_MSG_RECV");
		int funcId;
		QDataStream s(data, IO_ReadOnly);
        s >> funcId;
        //u3log_debug("funcId = [%d]",funcId);
		
        Q_UINT32 seqId;
        TransmitPartA partA;
        TransmitPartB partB;
        QByteArray param;
        s >> seqId >> partA >> partB >> param;
		
		QString address = TBusinessGlobal::convertDevAddrToString(partA.protocolType,partB.destAddr,&partB.recognizeType,&partB.recognizeId);
		u3log_debug(" address<%s> ",address.latin1());
		
		Q_UINT32 resetmode;
		int eventId;
		unsigned int isOpenspk;
		QDataStream d(param, IO_ReadOnly);
		d >> resetmode >> eventId >> isOpenspk;
		u3log_debug("eventId[%d]",eventId);
		u3log_debug("m_TimerState[%d]",m_TimerState);
		//Receive check TGID01 ,clean the timer state
		if (address == "2147483649")
		{
			m_TimerState =  Idle;
			u3log_debug("Set m_TimerState to Idle");
		}
		
		if(eventId == EVT_PAL_INCALL_SETUP &&
		   address == "2147483650" && 
	       (m_param.SQType  == MFG_BER || m_param.SQType  == Golden_BER))
		{
			if(Idle == m_TimerState )
			{
				u3log_debug("starting Squelch Timer");
				
				m_TimerState =  Running;
				// set value to starting Squelch Timer
				m_DSPControl->StartSquelchTuneSet(m_param.SQTimeout);
				
				//clear the Ber List
				m_ListberS.clear();
				//clear the RSSI List
				m_ListRssi.clear();
				
				//start BER &RSSI tune
				QCopEnvelope e("DSPEngine/DumpBuffer", "startMenuShowSignal()");
				//start get ber for every one second
				TAutoWorkStationRequest::instance()->startGetBER(REFRESHTIME);
			}	
    }
		
		if(eventId == EVT_PAL_EOC &&
		   address == "2147483650" &&
		  (m_param.SQType  == MFG_BER || m_param.SQType  == Golden_BER))
		{
			if(Running == m_TimerState )
			{
				u3log_debug("stop Squelch Timer(EOC)");
				
				m_DSPControl->StopSquelchTuneSet();
				m_TimerState = Finish ;
				//Notify UI to change color
				QCopEnvelope e1("G4/MFGTune", "SquelchTimeout()");
				//stop get the BitErrorRate & RSSI
				TAutoWorkStationRequest::instance()->stopGetBER();
				//stop BER &RSSI tune
				QCopEnvelope e("DSPEngine/DumpBuffer", "stopMenuShowSignal()");
				u3log_debug("m_SQType = %d",m_param.SQType);
				
				double avgRssi = 0;
				//RSSI
				for(unsigned int i = 0; i < m_ListRssi.count(); i++)
    {
					u3log_debug("RSSI[%d] = %f",i,m_ListRssi[i]);
					avgRssi += (double)m_ListRssi[i];
				}
				if(m_ListRssi.count() != 0)
				{
					//Average RSSI
					avgRssi = avgRssi/m_ListRssi.count();
					u3log_debug("AvgRssi[%f]",avgRssi);
				}
				//BER
				for(unsigned int i = 0; i < m_ListberS.count(); i++)
				{
					u3log_debug("BER[%d] = %f",i,m_ListberS[i].toFloat());	
				}
				
				//the Signal is unstable ,so set the ber to 100%
				double avgBer = 100;
				//write to file
				ToolForMfg::WriteBERResultToFile(m_param.SQFreq,m_param.SQLevel,avgBer,m_param.SQPassLevel,avgRssi);
				
				// Set Speaker to Auto
				TShareMemory().m_shm->spkDev[0] = SpkAudDev_Auto;
			}
		}
        
    }
}

/**
 *
 *  @author  Alex
 *
 *  @date    2016/07/06
 *
 *  @fn      void TMFGTuneMode::slt_13MHzResult(unsigned short dac)
 *
 *  @brief   ( receive 13MHz Tune result )
 *
 *  @param   ( dac : result )
 *
 *  @return  none
 *
 *  @note    none
 *
 */
void TMFGTuneMode::slt_13MHzResult(unsigned short dac)
{
    u3log_debug("======== TMFGTuneMode::slt_13MHzResult ========");

    u3log_debug("13MHz result : <%d>", dac);
    m_13MHzDAC = dac;
    
    //Back To before Mode
    m_OtaCHswitch->SetRecoveryOTAMode();
}

/**
 *
 *  @author  Shawn
 *
 *  @date    2017/07/25
 *
 *  @fn      void TMFGTuneMode::slt_SquelchTimeout()
 *
 *  @brief   ( receive Squelch Timeout )
 *
 *  @return  none
 *
 *  @note    none
 *
 */
void TMFGTuneMode::slt_SquelchTimeout()
{
    u3log_debug("======== TMFGTuneMode::slt_SquelchTimeout ========");

	m_TimerState = Finish ;
	//Notify UI to change color
	QCopEnvelope e1("G4/MFGTune", "SquelchTimeout()");
	//stop get the BitErrorRate & RSSI
	TAutoWorkStationRequest::instance()->stopGetBER();
	//stop BER &RSSI tune
	QCopEnvelope e("DSPEngine/DumpBuffer", "stopMenuShowSignal()");
	u3log_debug("m_SQType = %d",m_param.SQType);
	if(Golden_Squelch == m_param.SQType)
	{
		//get average RSSI
		double avgRssi = 0;
		u3log_debug("m_ListRssi.count() = %d",m_ListRssi.count());
		for(unsigned int i = 0; i < m_ListRssi.count(); i++)
		{
			u3log_debug("RSSI[%d] = %f",i,m_ListRssi[i]);
			avgRssi += (double)m_ListRssi[i];
		}
		if(m_ListRssi.count() != 0)
		{
			avgRssi = avgRssi/m_ListRssi.count();
			u3log_debug("AvgRssi[%f]",avgRssi);
		}
		
		//write RSSI to file
		ToolForMfg::WriteRSSIResultToFile(m_param.SQFreq,m_param.SQLevel,avgRssi);
	}
	else //MFG_BER & Golden_BER
	{
		double avgRssi = 0;
		float avgBer = 0;
		int vaildCount = 0;
		//RSSI
		for(unsigned int i = 0; i < m_ListRssi.count(); i++)
		{
			u3log_debug("RSSI[%d] = %f",i,m_ListRssi[i]);
			avgRssi += (double)m_ListRssi[i];
		}
		if(m_ListRssi.count() != 0)
		{
			//Average RSSI
			avgRssi = avgRssi/m_ListRssi.count();
			u3log_debug("AvgRssi[%f]",avgRssi);
		}
		//BER
		for(unsigned int i = 0; i < m_ListberS.count(); i++)
		{
			u3log_debug("BER[%d] = %f",i,m_ListberS[i].toFloat());
			//1~3 sec is unstable ber
			if(i > 2)
			{
				//"-1" is error ber
				if("-1" != m_ListberS[i])
				{
					avgBer += m_ListberS[i].toFloat();
					vaildCount ++;
				}
			}
				
		}
		if(vaildCount != 0)
		{
			//Average Bit Error Rate
			avgBer = avgBer/vaildCount;
			u3log_debug("AvgBer[%f]",avgBer);
		}
		
		/* berL
		avgBer = m_ListBerL.toFloat();
		u3log_debug("AvgBer[%f]",avgBer);
		*/
		if(avgBer < m_param.SQPassLevel)
		{
			//pass!! open spk
			u3log_debug("pass open spk");
			TAudioPackerV2::instance()->openSpk(Fun_Recv_VoiceCall, 1, 100);
		}

		//write to file
		ToolForMfg::WriteBERResultToFile(m_param.SQFreq,m_param.SQLevel,avgBer,m_param.SQPassLevel,avgRssi);
	}

    // Set Speaker to Auto
    TShareMemory().m_shm->spkDev[0] = SpkAudDev_Auto;
}
/**
 *
 *  @author  Shawn
 *
 *  @date    2017/07/25
 *
 *  @fn      TMFGTuneMode::slt_getBER()
 *
 *  @brief   ( get Bit Error Rate )
 *
 *  @return  none
 *
 *  @note    none
 *
 */
void TMFGTuneMode::slt_getBerRssi(const QCString &msg, const QByteArray &data)
{
	if(m_testMode)
	{
		TShareMemory mem;
		QString rateS,rateL,errorPct;
		// 100000 = 100%
		rateS.sprintf("%d.%03d",mem.m_shm->diagnosticInfo.bitErrorRateS/1000,mem.m_shm->diagnosticInfo.bitErrorRateS%1000);
		rateL.sprintf("%d.%03d",mem.m_shm->diagnosticInfo.bitErrorRateL/1000,mem.m_shm->diagnosticInfo.bitErrorRateL%1000);
		errorPct.sprintf("%d.%03d",mem.m_shm->diagnosticInfo.errorPct/1000,mem.m_shm->diagnosticInfo.errorPct%1000);
		QString berS = mem.m_shm->diagnosticInfo.bitErrorRateS >= 0xffff ? tr("-1") : rateS ;
		QString berL = mem.m_shm->diagnosticInfo.bitErrorRateL >= 0xffff ? tr("-1") : rateL ;
		QString erPct = mem.m_shm->diagnosticInfo.errorPct >= 0xffff ? tr("-1") : errorPct ;

		ToolForMfg::WriteRSSIToFile(mem.m_shm->diagnosticInfo.rssi/128,erPct.toInt(),berS.toInt(), berL.toInt());
		return ;
	}
	if(Golden_Squelch == m_param.SQType)
	{
		Q_INT32 rssi = 0;
		if(TBoard::getLastVoiceCallRssi(rssi))
		{
			m_ListRssi << (double)rssi;
			u3log_debug("RSSI[%d]",rssi);
		}
	}
	else if (MFG_BER == m_param.SQType || Golden_BER == m_param.SQType)
{
		//SDiagnostic sigInfo = TShareMemory().m_shm->diagnosticInfo;
		
		int bitErrorRate = TBoard::getBitErrorRateS();
		QString rateS;
		// 100000 = 100%
		rateS.sprintf("%d.%03d",bitErrorRate/1000,bitErrorRate%1000);
		QString berS = bitErrorRate >= 0xffff ? tr("-1") : rateS ;
		m_ListberS << berS ;
		u3log_debug("BER[%s %]",berS.latin1());
		/* berL
		int bitErrorRate = TBoard::getBitErrorRateL();
		QString rateL;
		rateL.sprintf("%d.%03d",bitErrorRate/1000,bitErrorRate%1000);
		QString berL = bitErrorRate >= 0xffff ? tr("-1") : rateL ;
		m_ListBerL = berL ;
		u3log_debug("m_ListBerL:%s",m_ListBerL.latin1());
		*/
		//RSSI
		Q_INT32 rssi = 0;
		if(TBoard::getLastVoiceCallRssi(rssi))
    {
			m_ListRssi << (double)rssi;
			u3log_debug("RSSI[%d]",rssi);
		}
	}
}

void TMFGTuneMode::slt_test(const QCString & msg, const QByteArray & data)
{
	u3log_debug("== TMFGTuneMode::slt_test ==[%s]",msg.data());
	//m_testTimer->start(5000,false);
	if(msg == "aaa()")
	{
		QCopEnvelope e("DSPEngine/DumpBuffer", "startMenuShowSignal()");
		TAutoWorkStationRequest::instance()->startGetBER(REFRESHTIME);
    }
	else if(msg == "bbb()")
    {
		TAutoWorkStationRequest::instance()->stopGetBER();
	}

	else if(msg == "test00()")
	{
		QCopEnvelope e("G4/MFGTune", "SquelchTune(QByteArray)");
		e << (Q_UINT32)0 << (Q_UINT32)0 << (int)0 << (float)0 << (Q_UINT32)0 << (Q_UINT32)0 <<(Q_UINT32)139500000 << (Q_UINT32)0;
    }
	else if(msg == "test01()")
	{
		QCopEnvelope e("G4/MFGTune", "SquelchTune(QByteArray)");
		e << (Q_UINT32)0 << (Q_UINT32)1 << (int)(-905) << (float)(5.8) << (Q_UINT32)12 << (Q_UINT32)0 <<(Q_UINT32)139500000 << (Q_UINT32)8;

}
	else if(msg == "test02()")
	{
		QCopEnvelope e("G4/MFGTune", "SquelchTune(QByteArray)");
		e << (Q_UINT32)0 << (Q_UINT32)2 << (int)(-805) << (float)0 << (Q_UINT32)12 << (Q_UINT32)0 <<(Q_UINT32)139500000 << (Q_UINT32)0;

	}
	else if(msg == "test03()")
	{
		QCopEnvelope e("G4/MFGTune", "SquelchTune(QByteArray)");
		e << (Q_UINT32)0 << (Q_UINT32)3 << (int)(-700) << (float)(5.9) << (Q_UINT32)12 << (Q_UINT32)0 <<(Q_UINT32)139500000 << (Q_UINT32)8;

	}
	else if(msg == "testModeStart()")
{
		u3log_debug("start get RSSI");
		TRingModeHandler::instance()->startFuncTone(eAudioBiz_BatteryLow, 0, 0);
		m_testMode = true;
		QCopEnvelope e("DSPEngine/DumpBuffer", "startMenuShowSignal()");
		//start get ber for every one second
		TAutoWorkStationRequest::instance()->startGetBER(REFRESHTIME);
	
	}
	else if(msg == "testModeStop()")
	{
		u3log_debug("stop get RSSI");
		TRingModeHandler::instance()->startFuncTone(eAudioBiz_BatteryLow, 0, 0);
		m_testMode = false;
		//stop BER &RSSI tune
		QCopEnvelope e("DSPEngine/DumpBuffer", "stopMenuShowSignal()");
		//stop get the BitErrorRate & RSSI
		TAutoWorkStationRequest::instance()->stopGetBER();
	}

	}

void TMFGTuneMode::slt_delayTimeout()
	{
	u3log_debug("==slt_delayTimeout==");
	// set value to starting Squelch Timer
	m_DSPControl->StartSquelchTuneSet(m_param.SQTimeout);
	//clear List
	m_ListRssi.clear();
	//start BER tune
	QCopEnvelope e("DSPEngine/DumpBuffer", "startMenuShowSignal()");
	//start get ber for every one second
	TAutoWorkStationRequest::instance()->startGetBER(REFRESHTIME);
	
	}

