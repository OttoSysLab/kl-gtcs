#pragma once

#include <qobject.h>
#include <qtopia/u3log.h>

#include "mfgtunedefine.h"
#include <qmap.h>
#include <qvaluevector.h>
#include <qtimer.h>
#include <qtopia/businesslogic/board.h>
#include <qlist.h>

class TAntennaTuneTable;
class TDSPControl;
class TReceiveconditionswitch;
class TCalibrateData;

class squelchOnOff
{
public:
	Q_UINT32 FreqStep;
	Q_UINT32 Freq;
	Q_UINT32 Level;
	double BER;
	bool 	 IsPass;
};
enum
{
	Idle = 0,
	Running = 1,
	Finish = 2,
};

class TMFGTuneMode : public QObject
{
    Q_OBJECT
    U3_LOG(TMFGTuneMode)
public:
    static TMFGTuneMode* instance(); 
    ~TMFGTuneMode();

private:
    TMFGTuneMode(QObject *parent = 0);

private slots:
    // receive ota order & test result order
    void slt_msgReceived(const QCString &msg, const QByteArray &data);
    // receive 13MHz result
    void slt_13MHzResult(unsigned short dac);
    // receive Squelch Timeout
    void slt_SquelchTimeout();

	void slt_test(const QCString &msg, const QByteArray &data);
	void slt_getBerRssi(const QCString &msg, const QByteArray &data);
	void slt_delayTimeout();
private:
    static TMFGTuneMode *m_instance;
    TAntennaTuneTable *m_AntennaTuneTable;
    TDSPControl *m_DSPControl;
    TReceiveconditionswitch *m_OtaCHswitch;
    TCalibrateData *m_TCalibrateData;

    AntennaTuneCondition m_Condition;
    Q_UINT32 m_DACTuneCount;
    unsigned short m_13MHzDAC;
    unsigned short m_13MhzStartDAC;
    Q_UINT32 m_13MHzUpperLimitDAC;
    Q_UINT32 m_13MHzLowerLimitDAC;
    
	QStringList m_ListberS;
	QString m_ListBerL;
	QValueList <double> m_ListRssi;
 
    MfgSquelchOnOffParameter m_param;
	
    QTimer*  m_testTimer;
	QTimer* m_DelayTimer;

	int m_TimerState;
	bool m_testMode;
};
