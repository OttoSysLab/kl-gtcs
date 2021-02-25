#include "interpolation.h"
#include "antennatunetable.h"

/**
 *
 *  @author  Alex
 *
 *  @date    2016/06/20
 *
 *  @fn      TAntennaTuneTable::TAntennaTuneTable(QObject *parent)
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
TAntennaTuneTable::TAntennaTuneTable(QObject *parent)
:QObject(parent)
{
    // create TInterpolation
    m_Interpolation = new TInterpolation(this);

    InitTable(35);
}

/**
 *
 *  @author  Alex
 *
 *  @date    2016/06/20
 *
 *  @fn      TAntennaTuneTable::~TAntennaTuneTable()
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
TAntennaTuneTable::~TAntennaTuneTable()
{  
}

/**
 *
 *  @author  Alex
 *
 *  @date    2016/06/20
 *
 *  @fn      void TAntennaTuneTable::InitTable(int count)
 *
 *  @brief   ( initial parameter )
 *
 *  @param   ( count : antenna tune count )
 *
 *  @return  none
 *
 *  @note    none
 *
 */
void TAntennaTuneTable::InitTable(int count)
{
    u3log_debug("==== TAntennaTuneTable::InitTable ====");

    m_AntennaTuneInfoList.clear();
    m_CurItem = 0;
    m_BetterDAC = 0;
    m_BetterRSSI = 0;
    m_DACTuneCount = count;

    // initial table
    for( int i = 1; i <= m_DACTuneCount; i++ )
    {
        AntennaTuneInfo antennaTuneInfo;

        antennaTuneInfo.item = i;
        antennaTuneInfo.condition = false;
        antennaTuneInfo.startDAC = 0;
        antennaTuneInfo.stopDAC = 0;
        antennaTuneInfo.stepDAC = 0;
        antennaTuneInfo.freq = 0;
        antennaTuneInfo.rssi = 0;
        antennaTuneInfo.DAC = 0;
        antennaTuneInfo.rssiLevel = 0;

        m_AntennaTuneInfoList << antennaTuneInfo;
    }
}

/**
 *
 *  @author  Alex
 *
 *  @date    2016/06/20
 *
 *  @fn      void SetRSSILevel(float rssilevel)
 *
 *  @brief   ( set rssilevel )
 *
 *  @param   ( item : item number )
 *           ( rssilevel : rssi level )
 *
 *  @return  none
 *
 *  @note    none
 *
 */
void TAntennaTuneTable::SetRSSILevel(int item, float rssilevel)
{
    u3log_debug("==== TAntennaTuneTable::SetRSSILevel(%f) ====", rssilevel);

    m_AntennaTuneInfoList[item-1].rssiLevel = rssilevel;
}

/**
 *
 *  @author  Alex
 *
 *  @date    2016/06/20
 *
 *  @fn      void TAntennaTuneTable::SetValue(int dac, float rssi)
 *
 *  @brief   ( set current dac and rssi )
 *
 *  @param   ( item : item )
 *           ( dac : dac )
 *           ( rssi : rssi )
 *
 *  @return  none
 *
 *  @note    none
 *
 */
void TAntennaTuneTable::SetValue(int item, int dac, float rssi)
{
    u3log_debug("==== TAntennaTuneTable::SetValue ====");

    if (false == m_AntennaTuneInfoList[item-1].condition)
    {
        // set condition
        m_AntennaTuneInfoList[item-1].condition = true;
        // set value
        m_AntennaTuneInfoList[item-1].DAC = dac;
        m_AntennaTuneInfoList[item-1].rssi = rssi;
    }
    else if (rssi > m_AntennaTuneInfoList[item-1].rssi)
    {
        // set value
        m_AntennaTuneInfoList[item-1].DAC = dac;
        m_AntennaTuneInfoList[item-1].rssi = rssi;
    }
}

/**
 *
 *  @author  Alex
 *
 *  @date    2016/06/20
 *
 *  @fn      void TAntennaTuneTable::Interpolation()
 *
 *  @brief   ( Interpolate )
 *
 *  @param   none
 *
 *  @return  none
 *
 *  @note    none
 *
 */
void TAntennaTuneTable::Interpolation()
{
    m_Interpolation->StructInterpolation(m_AntennaTuneInfoList);
}

/**
 *
 *  @author  Alex
 *
 *  @date    2016/07/18
 *
 *  @fn      int TAntennaTuneTable::CheckTable()
 *
 *  @brief   ( Check RSSI Level and Linear )
 *
 *  @param   ( checkno : condition item no. )
 *
 *  @return  ( Correct )
 *
 *  @note    none
 *
 */
bool TAntennaTuneTable::CheckTable(int &checkno)
{
    u3log_debug("==== TAntennaTuneTable::CheckTable ====");

    int dac = 0;
    checkno = 0;
    bool result = true;

    for(int i = 0; i < m_DACTuneCount; i++)
    {
        // rssi level check
        if (m_AntennaTuneInfoList[i].condition == true)
        {
            checkno++;

            if (m_AntennaTuneInfoList[i].rssiLevel > m_AntennaTuneInfoList[i].rssi)
            {
                u3log_error("rssi level error, item<%d> rssi<%f> rssilevel<%f>", m_AntennaTuneInfoList[i].item, m_AntennaTuneInfoList[i].rssi, m_AntennaTuneInfoList[i].rssiLevel);

                result = false;
            }
        }

        // linear check
        if (dac > m_AntennaTuneInfoList[i].DAC)
        {
            u3log_error("linear error, item<%d> olddac<%d> dac<%d>", m_AntennaTuneInfoList[i].item, dac, m_AntennaTuneInfoList[i].DAC);

            result = false;
        }

        dac = m_AntennaTuneInfoList[i].DAC;
    }

    return result;
}

/**
 *
 *  @author  Alex
 *
 *  @date    2016/06/20
 *
 *  @fn      AntennaTuneInfoList TAntennaTuneTable::GetTable()
 *
 *  @brief   ( GetTable )
 *
 *  @param   none
 *
 *  @return  Table
 *
 *  @note    none
 *
 */
AntennaTuneInfoList TAntennaTuneTable::GetTable()
{
    return m_AntennaTuneInfoList;
}

/**
 *
 *  @author  Alex
 *
 *  @date    2016/06/20
 *
 *  @fn      void TAntennaTuneTable::printTable()
 *
 *  @brief   ( Print Table )
 *
 *  @param   none
 *
 *  @return  none
 *
 *  @note    none
 *
 */
void TAntennaTuneTable::printTable()
{
    u3log_debug("==== TAntennaTuneTable::printTable ====");

    for( int j = 0; j < m_AntennaTuneInfoList.count(); j++ )
    {
        u3log_error("======== item<%02d> ======== freq<%u> ======== RSSI<%10.6f> ======== DAC<%5d>========" , m_AntennaTuneInfoList[j].item, m_AntennaTuneInfoList[j].freq, m_AntennaTuneInfoList[j].rssi, m_AntennaTuneInfoList[j].DAC);
    }
}
