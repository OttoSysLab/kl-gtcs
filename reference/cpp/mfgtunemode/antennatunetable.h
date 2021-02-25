#pragma once

#include <qobject.h>
#include <qtopia/u3log.h>

#include "interpolation.h"


class TAntennaTuneTable : public QObject
{
    Q_OBJECT
    U3_LOG(TMFGTuneMode)
public:
    TAntennaTuneTable(QObject *parent = 0); 
    ~TAntennaTuneTable();
  
    // reset table
    void InitTable(int count);
    // set rssilevel
    void SetRSSILevel(int item, float rssilevel);
    // get new dac & rssi
    void SetValue(int item, int dac, float rssi);
    // interpolate table
    void Interpolation();
    // Check Correct
    bool CheckTable(int &checkno);
    // get table
    AntennaTuneInfoList GetTable();
    // Print Table
    void printTable();

private:
    // interpolation method
    TInterpolation *m_Interpolation;
    // table
    AntennaTuneInfoList m_AntennaTuneInfoList;
    // Antenna Tune Count
    int m_DACTuneCount;
    // running item
    int m_CurItem;
    // dac at better rssi from start to now
    int m_BetterDAC;
    // better rssi from start to now
    float m_BetterRSSI;

};
