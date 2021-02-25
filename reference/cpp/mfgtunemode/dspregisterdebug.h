#pragma once 

#include <qobject.h>
#include <qtopia/u3log.h>
#include <qvaluelist.h>
#include <stdio.h>


class TDSPRegisterDebug : public QObject
{
    Q_OBJECT
    U3_LOG(TDSPRegisterDebug)
public:
    TDSPRegisterDebug(QObject *parent = 0);
    ~TDSPRegisterDebug();
    void StartVolumeTest(int time);
    static TDSPRegisterDebug * instance();
  
    void StartRssiLog(int time = 100);
    void StopRssiLog(); 
private slots:
    void slt_RunTestTimeout();
    void slt_PackLogDataToAdcFile();
    void slt_readRegisterTimeout();
    void slt_CopyFileToAdcFile();
    void slt_CopyFileToPackFile();
    void slt_PackDataFunc();
	void slt_received(const QCString& msg, const QByteArray& data);
    void slt_AutoLogBackup();
    void slot_debugrssi(const QCString &msg, const QByteArray &data);
    void slt_RSSILogTimer();
private:
    int GetVolumeTestVaule();
    void tryStartRegister();
    void writeRegisterLogTitle();
    void writeRegisterLog(QString s);
    bool readRegisterValue(int regId,int &value);
    void StartGetLogTimer( int setSec );
    void InitRegisterData();
    void CompareOneHourTime();
    bool IsFileExists( QString path );
    void CheckU3logHasOldFile();
    QString GetDateTimeFileName();
    void CheckSystemMessageHasOldFile();
    void GetTestConfigData();
    void writeToFile(int Count,int Time);
    
    void Check_RSSILOG_Path();
    void writeRSSILog();    
private:
    static TDSPRegisterDebug * m_instance;
    QTimer *m_VolumeTimer;
    int m_TestInterval;
    int m_CurrentTestTime;
    int m_TestValue;
    int m_TestDelayTime;
    bool m_IsRetryFunction;
    QTimer *m_GetLogTimer;
    QTimer *m_readRegisterTimer;
    QValueList<int> m_registerIdList;
    QString m_savePath;
    
    FILE *s_fpR;
    //FILE *s_fpRSSI;
    bool m_TestEnable;
	bool m_ATSLogEnable;
	int  m_BackupTime;
	bool m_DspLogEnable;
	bool m_AppsLogEnable;
	bool m_SysLogEnable;
	bool m_ArmTraceLogEnable;
	int m_count;
    QTimer *m_AutoLogTimer;
    QTimer *m_RSSILogTimer;
};

