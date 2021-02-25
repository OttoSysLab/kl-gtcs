#pragma once

#include <qobject.h>
#include <qtopia/u3log.h>

#include "mfgtunedefine.h"

class TCalibrateData : public QObject
{
    Q_OBJECT
    U3_LOG(TMFGTuneMode)
public:
    TCalibrateData(QObject *parent = 0);
    ~TCalibrateData();

    // write 13MHz result
    bool Write13MHzResult(unsigned short dac);
    // write AntennaTuneInfoList data to calibration data
    bool WriteCalibrateData( AntennaTuneInfoList info );

private:
    // get calibration data
    void GetCalibraionData(QByteArray &caldata);
    // set data to calibration data
    void SetAntennaTuneData(AntennaTuneInfoList info, QByteArray &caldata);
    // set data to calibration data
    void SetCheckSum(QByteArray &caldata);
    // write data to calibration
    void SetCalibrationData(QByteArray &caldata);

private:
    int m_RfcalLen;
};
