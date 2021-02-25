#include <qtopia/businesslogic/globaldefine.h>
#include <qtopia/dspinterface/dspinterface.h>
#include <qtopia/qcopenvelope_qws.h>
#include <qtopia/businesslogic/recvtableconfigpacker.h>
#include <qtopia/businesslogic/autoworkstationrequest.h>
#include <qtopia/businesslogic/chinfomanager2.h>
#include <qtopia/transmitscheduler1/chconfigparammanager.h>

#include "dspcontrol.h"

/**
 *
 *  @author  Alex
 *
 *  @date    2016/06/20
 *
 *  @fn      TDSPControl::TDSPControl(QObject *parent)
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
TDSPControl::TDSPControl(QObject *parent)
:QObject(parent)
{
    m_BestRssi = 0;
    m_BestDAC = 0;
    m_CurDAC = 0;
    m_RetryTimes = 0;

    m_13MHzTimeInterval = 0;

    m_13MHzTimer = new QTimer(this);
    connect(m_13MHzTimer, SIGNAL(timeout()), this, SLOT(slt_13MHzTimeout()));

    //m_SquelchTimer = new QTimer(this);
    //connect(m_SquelchTimer, SIGNAL(timeout()), this, SLOT(slt_SquelchTimeout()));
	
	// receive qcop signal
    QCopChannel *ch = new QCopChannel("G4/DSPControl", this);
    connect(ch, SIGNAL(received(const QCString&, const QByteArray&)), 
        this, SLOT(slt_msgReceived(const QCString &, const QByteArray &)));
}

/**
 *
 *  @author  Alex
 *
 *  @date    2016/06/20
 *
 *  @fn      TDSPControl::~TDSPControl()
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
TDSPControl::~TDSPControl()
{
}

/**
 *
 *  @author  Alex
 *
 *  @date    2016/06/21
 *
 *  @fn      void TDSPControl::StartAntennaTuneSet(AntennaTuneCondition condition)
 *
 *  @brief   ( record condition value & initial parameter)
 *
 *  @param
 *
 *  @return  none
 *
 *  @note    none
 *
 */
void TDSPControl::StartAntennaTuneSet(AntennaTuneCondition condition)
{
    u3log_debug("======== TDSPControl::StartSet ========");

    m_AntennaTuneCondition = condition;
    m_BestRssi = 0;
    m_BestDAC = 0;
    m_CurDAC = m_AntennaTuneCondition.startDAC;
    m_RetryTimes = 0;

    SetAntennaTune(m_AntennaTuneCondition.freq, m_CurDAC, m_AntennaTuneCondition.sampleRate);
}

/**
 *
 *  @author  Alex
 *
 *  @date    2016/06/21
 *
 *  @fn      bool TDSPControl::NextAntennaTuneSet(int &item, int &dac, float &rssi)
 *
 *  @brief   ( get result to change parameter )
 *
 *  @param   ( item : test item )
 *           ( dac : test dac )
 *           ( rssi : return rssi value )
 *
 *  @return  bool : this item is over?
 *
 *  @note    none
 *
 */
bool TDSPControl::NextAntennaTuneSet(int &item, int &dac, float &rssi)
{
    u3log_debug("======== TDSPControl::NextSet ========");
    u3log_debug("item<%u> dac<%u> rssi<%f>", m_AntennaTuneCondition.item, m_CurDAC, rssi);

    // First Set
    if (m_CurDAC == m_AntennaTuneCondition.startDAC ||
        (m_CurDAC != m_AntennaTuneCondition.startDAC && m_BestRssi < rssi))
    {
        m_BestRssi = rssi;
        m_BestDAC = m_CurDAC;
    }

    rssi = m_BestRssi;
    dac = m_BestDAC;
    item = m_AntennaTuneCondition.item;

    // show antenna tune
    QCopEnvelope e("G4/MFGTune", "AntennaTuneShow(QByteArray)");
    e << m_AntennaTuneCondition.freq << m_AntennaTuneCondition.item << m_AntennaTuneCondition.startDAC << m_AntennaTuneCondition.stepDAC << m_AntennaTuneCondition.stopDAC << m_CurDAC << dac << rssi;

    m_RetryTimes++;
    if (m_RetryTimes < RETRYTIMES)
    {
        GetAntennaTuneRssiValue(m_AntennaTuneCondition.sampleRate);

        return true;
    }
    else
    {
        // Check over
        m_CurDAC += m_AntennaTuneCondition.stepDAC;
        if (m_CurDAC <= m_AntennaTuneCondition.stopDAC)
        {
            m_RetryTimes = 0;
            // Set Next
            SetAntennaTune(m_AntennaTuneCondition.freq, m_CurDAC, m_AntennaTuneCondition.sampleRate);

            return true;
        }
        else
        {
            // RX_FE_DISABLE
            u3log_debug("palRX_FE_DISABLEReq()");
            palRX_FE_DISABLEReq();
            
            TChInfoManager2::notifyOtherRefresh();
            TChannelConfigParamManager::instance()->init();
            TRecvTableConfigPacker::instance()->changeRecvTable();
        }
    }

    return false;
}

/**
 *
 *  @author  Alex
 *
 *  @date    2016/07/06
 *
 *  @fn      unsigned short TDSPControl::Start13MHzTuneSet(int time)
 *
 *  @brief   ( 13MHz Test Start )
 *
 *  @param   ( time : minute )
 *
 *  @return  dac
 *
 *  @note    none
 *
 */
unsigned short TDSPControl::Start13MHzTuneSet( int time )
{
    u3log_debug("======== TDSPControl::Start13MHzTuneSet ========");

    m_13MHzTimeInterval = time * 60 * 1000;

    if (m_13MHzTimeInterval > 1000)
    {
        m_13MHzTimer->start(1000, true);
    }
    else
    {
        m_13MHzTimer->start(m_13MHzTimeInterval, true);
    }

    return Get13MHzValue();
}

/**
 *
 *  @author  Alex
 *
 *  @date    2016/07/06
 *
 *  @fn      void TDSPControl::slt_13MHzTimeout()
 *
 *  @brief   ( 13MHz Timeout )
 *
 *  @return  none
 *
 *  @note    none
 *
 */
void TDSPControl::slt_13MHzTimeout()
{
    u3log_debug("======== TDSPControl::slt_13MHzTimeout ========");

    unsigned short dac = Get13MHzValue();
    int over = 0;

    if (m_13MHzTimeInterval > 1000)
    {
        m_13MHzTimeInterval -= 1000;

        if (m_13MHzTimeInterval > 1000)
        {
            m_13MHzTimer->start(1000, true);
        }
        else
        {
            m_13MHzTimer->start(m_13MHzTimeInterval, true);
        }

        over = 0;
    }
    else
    {
        m_13MHzTimeInterval = 0;

        m_13MHzTimer->stop();

        over = 1;

        // send result
        emit sig_13MHzTuneStop(dac);
    }

    // Show UI
    QCopEnvelope e("G4/MFGTune", "T13MHzFreqTune(int)");
    e << dac << over;
}

/**
 *
 *  @author  Alex
 *
 *  @date    2016/07/05
 *
 *  @fn      unsigned short TDSPControl::Get13MHzValue()
 *
 *  @brief   ( get 13MHz value )
 *
 *  @return  value
 *
 *  @note    none
 *
 */
unsigned short TDSPControl::Get13MHzValue()
{
    u3log_debug("======== TDSPControl::Get13MHzValue ========");

    unsigned short value = 0;

    register_access_t reg;
    reg.reg_id = 2066;

    if (getDspRegister(&reg) != 0)
    {
        u3log_error("get DSP Register fail!");

        return 0;
    }
    else
    {
        value = reg.reg_data[1] * 256 + reg.reg_data[0];

        u3log_debug("13MHz vaule : <%d>", value);

        return value;
    }
}

/**
 *
 *  @author  Alex
 *
 *  @date    2016/06/21
 *
 *  @fn      void TDSPControl::SetAntennaTune(int freq, int dac, int sample)
 *
 *  @brief   ( use parameter to set dsp )
 *
 *  @param   ( freq : test frequency )
 *           ( dac : test dac )
 *           ( sample : test sample rate )
 *           ( generaltest : general test )
 *
 *  @return  none
 *
 *  @note    none
 *
 */
void TDSPControl::SetAntennaTune(int freq, int dac, int sample)
{
    u3log_debug("======== TDSPControl::SetAntennaTune ========");
    u3log_debug("Frequency<%d>  DAC<%d>", freq, dac);

    // CC1125_OVERRIDE
    struct palCC1125_OVERRIDE p1;
    p1.AGC_REF_override = 1;
    p1.AGC_REF = 127;
    p1.AGC_CSTHR_override = 1;
    p1.AGC_CSTHR = 96;
    p1.AGC_CFG3_override = 0;
    p1.AGC_CFG3 = 0;
    p1.AGC_CFG2_override = 0;
    p1.AGC_CFG2 = 0;
    p1.AGC_CFG1_override = 0;
    p1.AGC_CFG1 = 0;
    p1.AGC_CFG0_override = 0;
    p1.AGC_CFG0 = 0;

    u3log_debug("AGC_REF_override:%u,AGC_REF:%u,AGC_CSTHR_override:%u,AGC_CSTHR:%u,"
                "AGC_CFG3_override:%u,AGC_CFG3:%u,AGC_CFG2_override:%u,AGC_CFG2:%u,"
                "AGC_CFG1_override:%u,AGC_CFG1:%u,AGC_CFG0_override:%u,AGC_CFG0:%u",
                p1.AGC_REF_override,p1.AGC_REF,p1.AGC_CSTHR_override,p1.AGC_CSTHR,p1.AGC_CFG3_override,p1.AGC_CFG3,
                p1.AGC_CFG2_override,p1.AGC_CFG2,p1.AGC_CFG1_override,p1.AGC_CFG1,p1.AGC_CFG0_override,p1.AGC_CFG0);
            
    palCC1125_OVERRIDEReq(&p1,sizeof(p1));

    // RX_FE_DISABLE
    u3log_debug("palRX_FE_DISABLEReq()");
    palRX_FE_DISABLEReq();

    // RX_FE_CONFIG
    struct palRX_FE_CONFIG p2;
    p2.Channel_frequency = freq;
    p2.Channel_BW = 0;
    p2.Spectral_inversion = 0;
    p2.Debug_option = 0;
    p2.Performance_Path = 0;

    u3log_debug("Channel_frequency:%u,Channel_BW:%u,Spectral_inversion:%u,Debug_option:%u,Performance_Path:%u",
                p2.Channel_frequency,p2.Channel_BW,p2.Spectral_inversion,p2.Debug_option,p2.Performance_Path);

    palRX_FE_CONFIGReq(&p2,sizeof(p2));

    // RX_FE_ENABLE
    u3log_debug("palRX_FE_ENABLEReq()");
    palRX_FE_ENABLEReq();

    // RFCAL_CONTROL_OVERRIDE
    struct palRFCAL_CONTROL_OVERRIDE p3;
    p3.LNA_Bypass = 0; 
    p3.LNA_Bypass_Setting = 0;
    p3.Tune_override = 0;
    p3.Tune_DAC_setting = 0;
    p3.Antenna_override = 1;
    p3.Antenna_DAC_setting = dac;

    u3log_debug("LNA_Bypass:%u,LNA_Bypass_Setting:%u,Tune_override:%u,Tune_DAC_setting:%u,"
                "Antenna_override:%u,Antenna_DAC_setting:%u",
                p3.LNA_Bypass,p3.LNA_Bypass_Setting,p3.Tune_override,p3.Tune_DAC_setting,p3.Antenna_override,p3.Antenna_DAC_setting);

    palRFCAL_CONTROL_OVERRIDEReq(&p3,sizeof(p3));

    GetAntennaTuneRssiValue(sample);
}

/**
 *
 *  @author  Alex
 *
 *  @date    2016/06/21
 *
 *  @fn      void TDSPControl::GetAntennaTuneRssiValue(int sample)
 *
 *  @brief   ( set dsp to get rssi)
 *
 *  @param   ( sample : test sample rate )
 *
 *  @return  none
 *
 *  @note    none
 *
 */
void TDSPControl::GetAntennaTuneRssiValue(int sample)
{
    // RX_PWR_MSR
    u3log_debug("palRX_PWR_MSReq()");
    struct palRX_PWR_MS p4;
    p4.Samples = sample;

    u3log_debug("Samples:%u",p4.Samples);
    palRX_PWR_MSReq(&p4,sizeof(p4));
}

/**
 *
 *  @author  Alex
 *
 *  @date    2016/08/08
 *
 *  @fn      void TDSPControl::StartSquelchTuneSet()
 *
 *  @brief   ( Squelch Test Start )
 *
 *  @param   ( time : seconds )
 *
 *  @return  none
 *
 *  @note    none
 *
 */
void TDSPControl::StartSquelchTuneSet( int time )
{
    u3log_debug("======== TDSPControl::StartSquelchTuneSet( %d ) ========", time);

	//autoWorkStation Timer
	TAutoWorkStationRequest::instance()->startSquelchTune(time);
    //m_SquelchTimer->start( time * 1000, true );
}

void TDSPControl::StopSquelchTuneSet()
{
    u3log_debug("======== TDSPControl::StopSquelchTuneSet ========");

	//autoWorkStation Timer
	TAutoWorkStationRequest::instance()->stopSquelchTune();
    //m_SquelchTimer->start( time * 1000, true );
}

/**
 *
 *  @author  Alex
 *
 *  @date    2016/08/08
 *
 *  @fn      void TDSPControl::slt_SquelchTimeout()
 *
 *  @brief   ( Squelch Timeout )
 *
 *  @return  none
 *
 *  @note    none
 *
 */
void TDSPControl::slt_SquelchTimeout()
{
    u3log_debug("======== TDSPControl::slt_SquelchTimeout ========");

    emit sig_SquelchTuneStop();
}

void TDSPControl::slt_msgReceived(const QCString& msg, const QByteArray& data)
{
	u3log_debug("== TDSPControl::slt_msgReceived ==[%s]",msg.data());
	if("squelchTimeout()" == msg)
	{
		slt_SquelchTimeout();
	}
	
}
