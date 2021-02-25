#pragma once 

#include <qobject.h>
#include <qtopia/u3log.h>

#include "mfgtunedefine.h"


class TDSPControl : public QObject
{
    Q_OBJECT
    U3_LOG(TMFGTuneMode)
public:    
    TDSPControl(QObject *parent = 0);
    ~TDSPControl();

    // record start value
    void StartAntennaTuneSet(AntennaTuneCondition condition);
    // set next test
    bool NextAntennaTuneSet(int &item, int &dac, float &rssi);
    // Start to Test 13MHz Tune
    unsigned short Start13MHzTuneSet(int time);
    // Start to Test Squelch On/Off Tune
    void StartSquelchTuneSet(int time);
	void StopSquelchTuneSet();

signals:
    void sig_13MHzTuneStop(unsigned short value);
    void sig_SquelchTuneStop();

private slots:
    void slt_13MHzTimeout();
    void slt_SquelchTimeout();
	void slt_msgReceived(const QCString& msg, const QByteArray& data);
    
private:
	
    // set dsp
    void SetAntennaTune(int freq, int dac, int sample);
    // get rssi
    void GetAntennaTuneRssiValue(int sample);
    // get 13MHz Value
    unsigned short Get13MHzValue();

private:
    // startting value
    AntennaTuneCondition m_AntennaTuneCondition;
    // best rssi from start to now
    float m_BestRssi;
    // dac at best rssi from start to now
    int m_BestDAC;
    // test dac
    int m_CurDAC;
    // retry Times
    int m_RetryTimes;
    // 13MHz Timer
    QTimer *m_13MHzTimer;
    // 13MHz Test Interval
    int m_13MHzTimeInterval;
    // Squelch Timer
    QTimer *m_SquelchTimer;
};

