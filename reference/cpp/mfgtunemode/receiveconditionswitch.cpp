#include "receiveconditionswitch.h"
#include <qtopia/businesslogic/chinfomanager2.h>

#include <qtopia/businesslogic/configpacker.h>
#include <qtopia/businesslogic/contactboard.h>
#include <qtopia/businesslogic/recvtableconfigpacker.h>
#include <qtopia/transmitscheduler1/chconfigparammanager.h>
#include "../configurator/recvtablelib/handlers/updatetxrxfreq_handler.h"
#include <qtopia/transmitscheduler1/dsprfconfigurator.h>
#include <qtopia/audioV2/audiodbaccess.h>
#include <qtopia/businesslogic/gidrecvbehaviorjudgement.h>
#include <qtopia/alertmode/ringmodehandler.h>
#include <stdlib.h>

#define ZONE_ONE 1
#define DEFINE_BW 1
#define ZONEMULTI 100
#define REPEATERMODE 0
#define NOREPEATERMODE 1

#define REPEATMONITORMODEON 1
#define REPEATMONITORMODEOFF 0

#define DEFINE_BANDWIDTH 1
#define DEFINE_FREQ 763000000
TReceiveconditionswitch::TReceiveconditionswitch(QObject *parent)
:QObject(parent)
{
    QCopChannel *debug = new QCopChannel("G5/Test", this);
    connect(debug, SIGNAL(received(const QCString&,const QByteArray&)),
        this, SLOT(slot_debug(const QCString&, const QByteArray&)));
        
    QCopChannel *otachswitch = new QCopChannel("G5/OTACH", this);
    connect(otachswitch, SIGNAL(received(const QCString&,const QByteArray&)),
        this, SLOT(slot_SetCHData(const QCString&, const QByteArray&)));  
         
    m_OldZone = ZONE_ONE;         
    m_OTAFreq = DEFINE_FREQ;  
    
    //for CSQ frequency and P25C 855000000
    m_map[765012500] = 101;
	m_map[775987500] = 102;
	m_map[851012500] = 103;
	m_map[869987500] = 104;
	m_map[855000000] = 107;
	m_map[136012500] = 201;
	m_map[155012500] = 202;
	m_map[173987500] = 203;
	AddressStr[0] = "CSQ";
	AddressStr[1] = "2Tone";
	AddressStr[2] = "5Tone";
	AddressStr[3] = "CSQ";
	AddressStr[4] = "DMR";
	AddressStr[5] = "MDC1200";
	AddressStr[6] = "CSQ";
	AddressStr[7] = "DVOA";
	AddressStr[8] = "P25C";
	
	DefAddressStr[0] = "0";
	DefAddressStr[1] = "0,5000,18000";
	DefAddressStr[2] = "00001";
	DefAddressStr[3] = "0";
	DefAddressStr[4] = "2147483649";
	DefAddressStr[5] = "57344";
	DefAddressStr[6] = "0";
	DefAddressStr[7] = "57344";
	DefAddressStr[8] = "2147483649";	
	

}

TReceiveconditionswitch::~TReceiveconditionswitch()
{
    
}
/***********************************************************************************************************************
 *
 * Copyright (C) 2012, UNICATION CO.,LTD.   All Rights Reserved
 *
 * @author Kay
 *
 * @date 2016/5/09
 *
 * @fn TChInfoManager::SetCHData
 *
 * @description  Set CH Data
 *      

typedef struct {
    int  Id;                  // ID
    QString ModelNo;          // Pager Model No
    int  Frequency;           // MFG control CH rx frequency
    Q_UINT16  Protocol;       // MFG control CH protocol
    Q_UINT16  SA;             // MFG control CH sub sudio/NAC/color code depend on protocol
    Q_UINT32  CallID;         // MFG control CH rx address(group call id)
} MFGControlChFreqInfo;
                     
 * @return  UserDefineMultiCHInfoList
 *
 **********************************************************************************************************************/  
void TReceiveconditionswitch::SetCHData(MFGControlChFreqInfo &mfgControlChInfo)
{
	u3log_debug("SetCHData");
	SaveCHData(mfgControlChInfo);
   //SetZone( ADJUSTMENT_OTA_ZONE_NO );
}

/***********************************************************************************************************************
 *
 * Copyright (C) 2012, UNICATION CO.,LTD.   All Rights Reserved
 *
 * @author Kay
 *
 * @date 2016/5/09
 *
 * @fn TChInfoManager::addMonitorChannelandAddress
 *
 * @description  add Monitor Channel and Address
 *      

typedef struct {
    int  Id;                  // ID
    QString ModelNo;          // Pager Model No
    int  Frequency;           // MFG control CH rx frequency
    Q_UINT16  Protocol;       // MFG control CH protocol
    Q_UINT16  SA;             // MFG control CH sub sudio/NAC/color code depend on protocol
    Q_UINT16  CallID;         // MFG control CH rx address(group call id)
} MFGControlChFreqInfo;
                     
 * @return  UserDefineMultiCHInfoList
 *
 **********************************************************************************************************************/  
void TReceiveconditionswitch::SaveCHData(MFGControlChFreqInfo &mfgControlChInfo)
{
    
  	Q_UINT32 code = 0;
  	//get Zone and ch
  	TZoneKnobInfo zk = TBoard::getLockedZoneKnob(true);
  	//set save old zone 
  	m_OldZone =  zk.zoneId;   
    //TZoneKnobId zoneKnobId = zk.zoneId*100+zk.knobId;
    
	//set SA
	if(mfgControlChInfo.Protocol == U3_ST_DMR||
	   mfgControlChInfo.Protocol == U3_ST_P25)
	{
		code = mfgControlChInfo.SA;
	}
	u3log_debug("code[%d]",code);
    //set Address
    QString Address_Str = QString::number(mfgControlChInfo.CallID);
	u3log_debug("protocolList()[%d]",mfgControlChInfo.Protocol);
	u3log_debug("Frequency()[%d]",mfgControlChInfo.Frequency);
	u3log_debug("SA()[%d]",mfgControlChInfo.SA);
	u3log_debug("CallID[%s]",Address_Str.latin1());
     //set OTA Zone DB Data   
     bool ret = TChInfoManager3::SetOTAZone(ADJUSTMENT_OTA_ZONE_NO ); 
     //set OTA Zone DB Data  false return function   
    if( false == ret)
    {
        u3log_debug("Set Zone db fail");
        return;
    }

    // Get sinadSquelch
    Config cfg("/opt/Qtopia/etc/default/Settings/Sina.conf", Config::File);
    cfg.setGroup("Setting");
    int sinadSquelch = cfg.readNumEntry("sinadSquelch", 12);
    u3log_debug("sinadSquelch[%d]",sinadSquelch);
         
    ret = TChInfoManager3::addSelectiveCallChannel( (TZoneId) ADJUSTMENT_OTA_ZONE_NO , (TKnobId)zk.knobId,QString("OTA Knob %1").arg(zk.knobId),code, mfgControlChInfo.Frequency,DEFINE_BW, (U3SignalingType)mfgControlChInfo.Protocol,Address_Str,m_map,sinadSquelch); 
    u3log_debug("addSelectiveCallChannel Ret[%d]",ret);
    
    ret = TChInfoManager3::addOTARecvAllCallAlert((TZoneId) ADJUSTMENT_OTA_ZONE_NO , (TKnobId)zk.knobId);
    u3log_debug("addOTARecvAllCallAlert Ret[%d]",ret);
    
    //Update function on off ch TBoard value
	TBoard::getAllFunctionOnOffOfCh(zk.chId);
    
    //set OTA CH DB Data true ,load DB to memay                                   
	if(ret)
	{
		TContactBoard::preData();
		TChannelConfigParamManager::instance()->init();
	}	  
}

/***********************************************************************************************************************
 *
 * Copyright (C) 2012, UNICATION CO.,LTD.   All Rights Reserved
 *
 * @author Kay
 *
 * @date 2016/5/09
 *
 * @fn TChInfoManager::addMonitorChannelandAddress
 *
 * @description  add Monitor Channel and Address
 *      

typedef struct {
    int  Id;                  // ID
    QString ModelNo;          // Pager Model No
    int  Frequency;           // MFG control CH rx frequency
    Q_UINT16  Protocol;       // MFG control CH protocol
    Q_UINT16  SA;             // MFG control CH sub sudio/NAC/color code depend on protocol
    Q_UINT16  CallID;         // MFG control CH rx address(group call id)
} MFGControlChFreqInfo;
                     
 * @return  UserDefineMultiCHInfoList
 *
 **********************************************************************************************************************/  
void TReceiveconditionswitch::SaveCHDataRSSI(MFGControlChFreqInfo &mfgControlChInfo)
{
    
  	Q_UINT32 code = 0;
  	//get Zone and ch
  	TZoneKnobInfo zk = TBoard::getLockedZoneKnob(true);
  	//set save old zone 
  	m_OldZone =  zk.zoneId;
  	
  	TZoneKnobId zoneKnobId = zk.zoneId*100+zk.knobId;   
    
	//set SA
	if(mfgControlChInfo.Protocol == U3_ST_DMR){
		code = mfgControlChInfo.SA;
	}else if(mfgControlChInfo.Protocol == U3_ST_P25){
		code = mfgControlChInfo.SA;
	}
	u3log_debug("code[%d]",code);
    //set Address
    QString Address_Str = ReadProtocolAddress(mfgControlChInfo.Protocol );//QString::number(mfgControlChInfo.CallID);
 
	
	u3log_debug("protocolList()[%d]",mfgControlChInfo.Protocol);
	u3log_debug("Frequency()[%d]",mfgControlChInfo.Frequency);
	u3log_debug("SA()[%d]",mfgControlChInfo.SA);
	u3log_debug("CallID[%s]",Address_Str.latin1());
    
    QValueList<U3SignalingType> protocolList; 
    TContactBoard::getListenProtocolList(protocolList, zoneKnobId);
    u3log_debug("ch Protocol~~~[%d]",protocolList[0]);
    
        
    TChannelRFInfo baseInfo;
    TBusinessGlobal::getCurChannelInfo(TBoard::lockedChannel(), baseInfo);
	u3log_debug("baseInfo.rxFreq.toInt()[%d]",baseInfo.rxFreq.toInt());
	
	
	
	if(( mfgControlChInfo.Protocol == (Q_UINT16) protocolList[0]) && ((baseInfo.rxFreq.toInt()) != mfgControlChInfo.Frequency)  
	    &&  (TBusinessGlobal::getCurSINADSquelchSetting() != m_sinad))
	{
	   u3log_debug("mfgControlChInfo");
       //Set Freq
       SetTxRxFreq(mfgControlChInfo.Frequency);   
	   return; 
	}    	  
    //gst zone data    

    TZoneBaseInfo zoneBaseInfo;
    //获取某个zoneid的信息
    TContactBoard::getZone(zk.zoneId,zoneBaseInfo);
     //set OTA Zone DB Data   
	bool ret = TChInfoManager3::SetOTAZone(zoneBaseInfo.zoneId,zoneBaseInfo.zoneName ); 
     //set OTA Zone DB Data  false return function   
    if( false == ret)
    {
        u3log_debug("Set Zone db fail");
        return;
    }        
    //set ch data            
    ret = TChInfoManager3::addSelectiveCallChannel( zoneBaseInfo.zoneId, 
                                        (TKnobId)zk.knobId,zoneBaseInfo.zoneName,code, mfgControlChInfo.Frequency,DEFINE_BW, (U3SignalingType)mfgControlChInfo.Protocol,Address_Str,m_map,m_sinad); 
    
    
    //Update function on off ch TBoard value
	TBoard::getAllFunctionOnOffOfCh(zk.chId);
    
    //set OTA CH DB Data true ,load DB to memay                                   
	if(ret){
	    u3log_debug("addSelectiveCallChannel Ret[%d]",ret);
		//TContactBoard::preData();
		TChannelConfigParamManager::instance()->init();
		 QCopEnvelope e("U4/UpdateContactBoard", "updateAll()");     
         //TChInfoManager2::notifyOtherRefresh();
        TRecvTableConfigPacker::instance()->changeRecvTable("", true);
	}	  
}

/***********************************************************************************************************************
 *
 * Copyright (C) 2012, UNICATION CO.,LTD.   All Rights Reserved
 *
 * @author Kay
 *
 * @date 2016/5/09
 *
 * @fn TChInfoManager::addMonitorChannelandAddress
 *
 * @description  add Monitor Channel and Address
 *      

                     
 * @return  UserDefineMultiCHInfoList
 *
 **********************************************************************************************************************/  
void TReceiveconditionswitch::SetZone(int Zone )
{
    u3log_debug("Zone()[%d]",Zone);
    TConfigPacker::instance()->configCurZone(ConfigOpcode_Update, Zone);    

}
/***********************************************************************************************************************
 *
 * Copyright (C) 2012, UNICATION CO.,LTD.   All Rights Reserved
 *
 * @author Kay
 *
 * @date 2016/5/09
 *
 * @fn TChInfoManager::ToBackZone
 *
 * @description  To Back Zone
 *      
 * @return  UserDefineMultiCHInfoList
 *
 **********************************************************************************************************************/  
void TReceiveconditionswitch::ToBackZone()
{
  	TZoneKnobInfo zk = TBoard::getLockedZoneKnob(true);
    TZoneKnobId zoneKnobId = zk.zoneId*ZONEMULTI+zk.knobId;
    u3log_debug("m_OldZone()[%d]",m_OldZone);
    if(m_OldZone == ADJUSTMENT_OTA_ZONE_NO )
    {
      m_OldZone = ZONE_ONE;  
    }    
    //del zone 
    TChInfoManager3::delOTAZone(ADJUSTMENT_OTA_ZONE_NO);
    //del  OTA Recv All Call Alert  
    TChInfoManager3::DelOTARecvAllCallAlert();    
    //set Zone    
    TConfigPacker::instance()->configCurZone(ConfigOpcode_Update, m_OldZone);  
      
    //reload data    
    TContactBoard::preData();
	TChannelConfigParamManager::instance()->init(); 
}
/***********************************************************************************************************************
 *
 * Copyright (C) 2012, UNICATION CO.,LTD.   All Rights Reserved
 *
 * @author Kay
 *
 * @date 2016/5/09
 *
 * @fn TChInfoManager::slot_SetCHData
 *
 * @description  To Back Zone
 *      
 * @return  UserDefineMultiCHInfoList
 *
 **********************************************************************************************************************/  
void TReceiveconditionswitch::slot_SetCHData(const QCString& msg, const QByteArray& data)
{
	::system("rm /tmp/RSSI /tmp/BitErrorRate");
	
	TZoneKnobInfo zk = TBoard::getLockedZoneKnob(true);
	TZoneKnobId zoneKnobId = zk.zoneId*ZONEMULTI+zk.knobId;
    u3log_debug("####zoneId[%d]####",zoneKnobId);
	
	u3log_debug("To Back Zone!!!!!!!!!");
      //kay add 2016/06/22 pm 02:04:51
    MFGControlChFreqInfo tmp_mfgControlChInfo;
    //get MFGControlChFreqInfo Data 
    if(  msg  == "init()")
    {    

    //read Applications/mfg_tune/mfg.db data to tmp_mfgControlChInfo 
    TBusinessGlobal::getMFGControlChInfo( tmp_mfgControlChInfo );
    u3log_debug("======== ModelNo[%s] ========",tmp_mfgControlChInfo.ModelNo.latin1()); 
    u3log_debug("======== Frequency[%d] ========",tmp_mfgControlChInfo.Frequency); 
    u3log_debug("======== Protocol[%d] ========",tmp_mfgControlChInfo.Protocol); 
    u3log_debug("======== SA[%d] ========",tmp_mfgControlChInfo.SA);
      
    u3log_debug("======== CallID[%s] ========",QString::number(tmp_mfgControlChInfo.CallID).latin1());   
   	
	//Set m_OTAFreq data
	m_OTAFreq = tmp_mfgControlChInfo.Frequency;    
    	
	//set CH    
	SetCHData(tmp_mfgControlChInfo); 
	
	if( zk.zoneId ==  ADJUSTMENT_OTA_ZONE_NO)
	{
		TRecvTableConfigPacker::instance()->changeRecvTable();	
	}
	else
	{
		TConfigPacker::instance()->configCurZone(ConfigOpcode_Update, ADJUSTMENT_OTA_ZONE_NO);	
	}
	//eAudioBiz_Ring alert set kay add
	TRingModeHandler::instance()->startFuncTone(eAudioBiz_BatteryLow, 0, 0);
	//reload data  Qpe will be delayed so a direct call Function   
	//TChannelConfigParamManager::instance()->init();   
	//(change CH)
	//TRecvTableConfigPacker::instance()->changeRecvTable();
    }
    else if(  msg  == "RSSILOG()")
    {
        QDataStream stream(data, IO_ReadOnly);
        int Frequency = 0;
        int Protocol = 0;
        //int sinad_index = 0;
        
        stream >> Frequency >> Protocol>> m_sinad ; 

        //read Applications/mfg_tune/mfg.db data to tmp_mfgControlChInfo 
        TBusinessGlobal::getMFGControlChInfo( tmp_mfgControlChInfo );
        u3log_debug("======== ModelNo[%s] ========",tmp_mfgControlChInfo.ModelNo.latin1()); 

        u3log_debug("======== SA[%d] ========",tmp_mfgControlChInfo.SA);
          
        u3log_debug("======== CallID[%s] ========",QString::number(tmp_mfgControlChInfo.CallID).latin1()); 
        tmp_mfgControlChInfo.Frequency = Frequency;    
        tmp_mfgControlChInfo.Protocol = Protocol;
        u3log_debug("======== Frequency[%d] ========,%d",tmp_mfgControlChInfo.Frequency,Frequency); 
        u3log_debug("======== Protocol[%d] ========,%d",tmp_mfgControlChInfo.Protocol,Protocol); 
        //TConfigPacker::instance()->configAudioSquelchSetting(ConfigOpcode_Update,(AudioSquelchLevel)sinad_index);
       	u3log_debug("======== m_sinad[%d] ========",m_sinad);
    	//Set m_OTAFreq data
    	m_OTAFreq = tmp_mfgControlChInfo.Frequency;  
    	
    	//set CH    
    	SaveCHDataRSSI(tmp_mfgControlChInfo); 

    	          
    }
}    


/***********************************************************************************************************************
 *
 * Copyright (C) 2012, UNICATION CO.,LTD.   All Rights Reserved
 *
 * @author Kay
 *
 * @date 2016/6/23
 *
 * @fn TChInfoManager::SetRepeaterMode
 *
 * @description  Set Repeater Mode
 *      
 * @return  N/A
 *
 **********************************************************************************************************************/  
void TReceiveconditionswitch::SetRepeaterMode()
{
    TZoneKnobInfo zk = TBoard::getLockedZoneKnob(true);
    TZoneKnobId zoneKnobId = zk.zoneId*ZONEMULTI+zk.knobId;
    //Set Repeater Mode

    TChInfoManager3::SetTalkAroundOnrMode((int) zoneKnobId,REPEATERMODE ,REPEATMONITORMODEOFF );
    //set  Board  Around On Off and MonitorMode On Off
    TBoard::setTalkAroundOnOff(REPEATERMODE);
    TBoard::setMonitorModeOnOff(REPEATMONITORMODEOFF);   
}

/***********************************************************************************************************************
 *
 * Copyright (C) 2012, UNICATION CO.,LTD.   All Rights Reserved
 *
 * @author Kay
 *
 * @date 2016/6/23
 *
 * @fn TChInfoManager::SetNoRepeaterMode
 *
 * @description  Set Np Repeater Mode
 *      
 * @return  N/A
 *
 **********************************************************************************************************************/ 
void TReceiveconditionswitch::SetNoRepeaterMode()
{   
    TZoneKnobInfo zk = TBoard::getLockedZoneKnob(true);
    TZoneKnobId zoneKnobId = zk.zoneId*ZONEMULTI+zk.knobId;
    //Set Repeater Mode
    TChInfoManager3::SetTalkAroundOnrMode((int) zoneKnobId,NOREPEATERMODE ,REPEATMONITORMODEON );
    //set  Board  Around On Off and MonitorMode On Off
    TBoard::setTalkAroundOnOff(NOREPEATERMODE);
    TBoard::setMonitorModeOnOff(REPEATMONITORMODEON);   
   
}


/***********************************************************************************************************************
 *
 * Copyright (C) 2012, UNICATION CO.,LTD.   All Rights Reserved
 *
 * @author Kay
 *
 * @date 2016/6/23
 *
 * @fn TChInfoManager::SetNoRepeaterMode
 *
 * @description  Set Np Repeater Mode
 *      
 * @return  N/A
 *
 **********************************************************************************************************************/ 
void TReceiveconditionswitch::SetTxRxFreq( const unsigned int Frequency)
{
    //get Zone and ch Knob
    TZoneKnobInfo zk = TBoard::getLockedZoneKnob(true);
    
    //Set Repeater Mode
    TUpdateTxRxFreq_Handler *h = new TUpdateTxRxFreq_Handler();
    //int zoneId = 1;
    //int knobId = 2;
    
    ConfigSFrequency param;
    param.txFrequency = Frequency;
    param.rxFrequency = Frequency;
    param.txBandWidth = DEFINE_BANDWIDTH;
    param.rxBandWidth = DEFINE_BANDWIDTH;
    u3log_debug("======== txFrequency[%d] ========",Frequency);
    bool ret = h->updateTxRxFreq(zk.zoneId,zk.knobId,param);
        
	delete h;

}
/***********************************************************************************************************************
 *
 * Copyright (C) 2012, UNICATION CO.,LTD.   All Rights Reserved
 *
 * @author Kay
 *
 * @date 2016/6/23
 *
 * @fn TChInfoManager::SetOTA13MHzMode
 *
 * @description  const unsigned int Frequency
 *      
 * @return  N/A
 *
 **********************************************************************************************************************/  
void TReceiveconditionswitch::SetOTA13MHzMode(const unsigned int Frequency )
{
    //Set Freq
    SetTxRxFreq(Frequency);

    //Set RepeaterMode()
    SetRepeaterMode();

        
    TChInfoManager2::notifyOtherRefresh();
         
    //reload data  Qpe will be delayed so a direct call Function   
    TChannelConfigParamManager::instance()->init();
    //(change CH)
    TRecvTableConfigPacker::instance()->changeRecvTable(); 
        
    
         
}

/***********************************************************************************************************************
 *
 * Copyright (C) 2012, UNICATION CO.,LTD.   All Rights Reserved
 *
 * @author Kay
 *
 * @date 2016/6/23
 *
 * @fn TChInfoManager::SetOTAMode
 *
 * @description  N/A
 *      
 * @return  N/A
 *
 **********************************************************************************************************************/  
void TReceiveconditionswitch::SetRecoveryOTAMode()
{
    //Set Freq
    SetTxRxFreq(m_OTAFreq);
    //Set No RepeaterMode()
    SetNoRepeaterMode();
    //reload data    
    TChInfoManager2::notifyOtherRefresh();
    

    //reload data  Qpe will be delayed so a direct call Function   
    TChannelConfigParamManager::instance()->init();
        
         
    //(change CH)
    TRecvTableConfigPacker::instance()->changeRecvTable();     

        
        
}

/***********************************************************************************************************************
 *
 * Copyright (C) 2012, UNICATION CO.,LTD.   All Rights Reserved
 *
 * @author Kay
 *
 * @date 2016/7/8
 *
 * @fn TChInfoManager::SetdisableRadio
 *
 * @description  N/A
 *      
 * @return  N/A
 *
 **********************************************************************************************************************/  
void TReceiveconditionswitch::SetdisableRadio()
{    
    u3log_debug("======== TReceiveconditionswitch::SetdisableRadio() ========");
    //disable Radio
    TDSPRFConfigurator::instance()->disableRadio();
}

/***********************************************************************************************************************
 *
 * Copyright (C) 2012, UNICATION CO.,LTD.   All Rights Reserved
 *
 * @author Kay
 *
 * @date 2016/8/09
 *
 * @fn TChInfoManager::SetSquelchTune
 *
 * @description  To Squelch On/Off OTA switch
 *      
 * @return  N/A
 *
 **********************************************************************************************************************/  
void TReceiveconditionswitch::SetSquelchTune(Q_UINT32 SQFreq,Q_UINT32 SQProtocol )
{
	TZoneKnobInfo zk = TBoard::getLockedZoneKnob(true);
	TZoneKnobId zoneKnobId = zk.zoneId*ZONEMULTI+zk.knobId;
    u3log_debug("####zoneId[%d]####",zoneKnobId);
	
    u3log_debug("======== Frequency[%d] ========",SQFreq); 
    u3log_debug("======== Protocol[%d] ========",SQProtocol); 
    u3log_debug("======== m_before_Freq[%d] ========",m_before_Freq);
    u3log_debug("======== m_before_Protocol[%d] ========",m_before_Protocol);        
    //kay add 2017/8/3 am 10:38:21,Freq on change ,No Reset Freq
    if(  (m_before_Freq == SQFreq) && (m_before_Protocol == SQProtocol) )
    {
       return;
    }    
    //kay add 2017/8/3 am 10:38:21,Save Freq before  SQFreq Data    
    m_before_Freq = SQFreq;
    m_before_Protocol = SQProtocol;    
	bool ret = TChInfoManager3::SetOTAZone(ADJUSTMENT_OTA_ZONE_NO ); 
	//set OTA Zone DB Data  false return function   
    if( false == ret)
    {
        u3log_debug("Set Zone db fail");
        return;
    }

    // Get sinadSquelch
    Config cfg("/opt/Qtopia/etc/default/Settings/Sina.conf", Config::File);
    cfg.setGroup("Setting");
    int sinadSquelch = cfg.readNumEntry("sinadSquelch", 12);
    u3log_debug("----------------------------sinadSquelch[%d]-----------------------------",sinadSquelch);

    ret = TChInfoManager3::addMutiCsqP25cChannel( (TZoneId) ADJUSTMENT_OTA_ZONE_NO, 
			(TKnobId)zk.knobId,QString("OTA Knob %1").arg(zk.knobId),659, SQFreq,DEFINE_BW, (U3SignalingType)SQProtocol,m_map, sinadSquelch); 
    u3log_debug("addMutiCsqP25cChannel Ret[%d]",ret);
    
    //Update function on off ch TBoard value
	TBoard::getAllFunctionOnOffOfCh((TChannelId)zoneKnobId);
    
	//update audio table
	TAudioDBAccessV2::instance()->initAll();
	//update tabRecvBehaviorByContactID
	TGIDRecvBehaviorJudgement::instance()->loadData();
    //set OTA CH DB Data true ,load DB to memay                                   
	if(ret)
	{
		TContactBoard::preData();
		TChannelConfigParamManager::instance()->init();
	}
	
    
    if( zk.zoneId ==  ADJUSTMENT_OTA_ZONE_NO)
	{
		TRecvTableConfigPacker::instance()->changeRecvTable();	
	}
	else
	{
		TConfigPacker::instance()->configCurZone(ConfigOpcode_Update, ADJUSTMENT_OTA_ZONE_NO);
	}
    //reload data  Qpe will be delayed so a direct call Function   
    //TChannelConfigParamManager::instance()->init();   
    //(change CH)
	//TRecvTableConfigPacker::instance()->changeRecvTable();         
    //Monitor Off
    //TConfigPacker::instance()->configMonitorOnOff(ConfigOpcode_Update,false);
} 

/***********************************************************************************************************************
 *
 * Copyright (C) 2012, UNICATION CO.,LTD.   All Rights Reserved
 *
 * @author Kay
 *
 * @date 2016/8/09
 *
 * @fn TChInfoManager::SetSquelchTune
 *
 * @description  To Squelch On/Off OTA switch
 *      
 * @return  N/A
 *
 **********************************************************************************************************************/  
QString TReceiveconditionswitch::ReadProtocolAddress(int Protocol )
{
    u3log_debug("ReadProtocolAddress");
    QString Address = "";
    Config cfg("/opt/Qtopia/etc/default/Settings/rssiscansetting.conf", Config::File);
    cfg.setGroup("Protocol");
    u3log_debug("AddressStr[%d]=%s",Protocol,AddressStr[Protocol].latin1());
    u3log_debug("DefAddressStr[%d]=%s",Protocol,DefAddressStr[Protocol].latin1());    
    Address = cfg.readEntry(AddressStr[Protocol].latin1(), DefAddressStr[Protocol].latin1());
    u3log_debug("Address:[%s]",Address.latin1());
    
    return Address;
  
}

/******************
 *
 * Copyright (C) 2012, UNICATION CO.,LTD.   All Rights Reserved
 *
 * @author Kay
 *
 * @date 2017/8/03
 *
 * @fn TChInfoManager::InitSquelchTuneData
 *
 * @description  To Init Squelch Tune Data
 *      
 * @return  N/A
 *
 ***************/  
void TReceiveconditionswitch::InitSquelchTuneData()
{
   m_before_Freq = 0; 
   m_before_Protocol = 100;
}    

void TReceiveconditionswitch::slot_debug(const QCString& msg, const QByteArray& data)
{
    u3log_debug(" TReceiveconditionswitch::slot_debug");
/* typedef struct {
    int  Id;                  // ID
    QString ModelNo;          // Pager Model No
    int  Frequency;           // MFG control CH rx frequency
    Q_UINT16  Protocol;       // MFG control CH protocol
    Q_UINT16  SA;             // MFG control CH sub sudio/NAC/color code depend on protocol
    Q_UINT32  CallID;         // MFG control CH rx address(group call id)
} MFGControlChFreqInfo;*/
 if( "666()"== msg)
 {   
    int Frequency = 865012500;
    Q_UINT16 Protocol = 8;
    int sinad = 4;
        
    //stream >> Frequency >> Protocol; 
    QCopEnvelope e("G5/OTACH", "RSSILOG()");
    e<<Frequency<<Protocol<<sinad;
   
  }
  if("777()"== msg)
  {
    int Frequency = 763000000;
    Q_UINT16 Protocol = 8;
    int sinad = 4;
        
    //stream >> Frequency >> Protocol; 
    QCopEnvelope e("G5/OTACH", "RSSILOG()");
    e<<Frequency<<Protocol<<sinad;
  }
  if("UUU()"== msg)
  {
    u3log_debug("UUU");
    for(int i =0;i<=8;i++)
    {
        ReadProtocolAddress(i);
    }
  }
  if("Frequency1()"== msg)
  {
    SetOTA13MHzMode(865012500);
  }
  if("Frequency2()"== msg)
  {
    SetOTA13MHzMode(763000000);
  }
  if("disableRadio()"== msg)
  {
    SetdisableRadio();
  }
  if( "AAA()"== msg)
  {
    SetSquelchTune(165000000,0);
  }
}

