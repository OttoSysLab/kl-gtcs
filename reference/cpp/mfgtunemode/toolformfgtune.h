#pragma once

#include <vector>

#include <qobject.h>
#include <qtopia/u3log.h>

#include "mfgtunedefine.h"

using namespace std;

class ToolForMfg  :  public QObject
{
    Q_OBJECT
    U3_LOG(TMFGTuneMode)

public:
    // create folder
    static void createMfgFolder();
    // write antenna tune result to file
    static void WriteAntennaTuneValueToFile( AntennaTuneInfoList info );
    // write 13 mhz tune result to file
    static void Write13MHzTuneValueToFile( unsigned short dac );
    // get check sum value
    static unsigned short crc_ccitt(unsigned short * ptr, int length);

	static bool WriteBERResultToFile(int frequency , int level , double BER , double AverBER, double RSSI);
	static bool WriteRSSIResultToFile(int frequency , int level , double RSSI);
	static bool WriteRSSIToFile(int RSSI,int errorPct,int BerS,int BerL);
};