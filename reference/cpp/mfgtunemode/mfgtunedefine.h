#pragma once

#include <qlist.h>


#define MfgTunePath     ("/runpara/Applications/mfg_tune/")
#define AntennaTunePath ("/runpara/Applications/mfg_tune/AntennaDAC")
#define T13MHzTunePath  ("/runpara/Applications/mfg_tune/AFCDAC")
#define G_16 ((1 << 12) | (1 << 5) | 1)
#define RETRYTIMES 1

struct AntennaTuneInfo
{
    int item;     
    bool condition;
    int startDAC; 
    int stopDAC;
    int stepDAC;
    int freq;   
    float rssi;   
    int DAC;
    float rssiLevel;
};

struct AntennaTuneCondition
{
    int item; 
    int startDAC; 
    int stopDAC;
    int stepDAC;
    int freq;   
    int sampleRate;    
};

typedef QValueList<AntennaTuneInfo> AntennaTuneInfoList;

