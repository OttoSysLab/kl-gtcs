#pragma once

#include <qobject.h>
#include <qtopia/u3log.h>
#include <qtopia/businesslogic/globaldefine.h>
#include <qmap.h>

#include "../public/businesslogic/board/chinfomanager3_pager.h"


class TReceiveconditionswitch  :  public QObject
{
    Q_OBJECT
    U3_LOG(TMFGTuneMode)
public:
    TReceiveconditionswitch(QObject *parent = 0);
    ~TReceiveconditionswitch();
    //set CH Data 
    void SetCHData(MFGControlChFreqInfo &mfgControlChInfo);
    //switch Zone 
    void SetZone(int Zone );
    //Back before Zone
    void ToBackZone();
    //Set OTA 13 MHz Mode
    void SetOTA13MHzMode(const unsigned int Frequency );
    //Set OTA Mode
    void SetRecoveryOTAMode();
    //disable Radio
    void SetdisableRadio();
    void SetSquelchTune(Q_UINT32 SQFreq,Q_UINT32 SQProtocol);
    
    void InitSquelchTuneData();   
private slots:
    void slot_debug(const QCString&, const QByteArray&); 
    void slot_SetCHData(const QCString&, const QByteArray&); 

private:
	static TReceiveconditionswitch* m_instance;
    //Save CH Data to DB
    void SaveCHData(MFGControlChFreqInfo &mfgControlChInfo);  
    //set G5 Frequency 
    void SetTxRxFreq( const unsigned int Frequency);    
    //Save CH Data RSSI log ch  to DB
    void SaveCHDataRSSI(MFGControlChFreqInfo &mfgControlChInfo);  
    //Set CH Repeater Mode
    void SetRepeaterMode();
    //Set CH No Repeater Mode
    void SetNoRepeaterMode();    
    
    QString ReadProtocolAddress(int Protocol ); 
private:
    int m_OldZone;
    unsigned int m_OTAFreq;
    
    FreqToChIdMap m_map;
    Q_UINT32 m_before_Freq;
    Q_UINT32 m_before_Protocol;

    int m_sinad;
    QString AddressStr[9]; 
    QString DefAddressStr[9]; 
};
