#include "interpolation.h"

/**
 *
 *  @author  William
 *
 *  @date    2016/06/21
 *
 *  @fn      TInterpolation::TInterpolation(QObject *parent)
 *
 *  @brief   ( Constructivist )
 *
 *  @param   QObject *parent
 *
 *  @return  none
 *
 *  @note    none
 *
 */
TInterpolation::TInterpolation(QObject *parent)
:QObject(parent)
{
}

/**
 *
 *  @author  William
 *
 *  @date    2016/06/21
 *
 *  @fn      TInterpolation::~TInterpolation()
 *
 *  @brief   ( Deconstructive )
 *
 *  @param   none
 *
 *  @return  none
 *
 *  @note    none
 *
 */
TInterpolation::~TInterpolation()
{
    
}

/**
 *
 *  @author  William
 *
 *  @date    2016/06/21
 *
 *  @fn      bool TInterpolation::StructInterpolation(AntennaTuneInfoList &info)
 *
 *  @brief   ( get AntennaTuneInfoList List )
 *
 *  @param   AntennaTuneInfoList &info
 *
 *  @return  ( is operator value )
 *
 *  @note    none
 *
 */
bool TInterpolation::StructInterpolation(AntennaTuneInfoList &info)
{
    u3log_debug("==== TInterpolation::StructInterpolation ====");

    // no value operator return false
    if( info.count() == 0 || info[0].condition == false)
    {
        return false;
    }

    bool check1 = true;
    bool check2 = true;
    QValueList<int*> plist;

    for ( unsigned int i = 0; i < info.count(); i++ )
    {
        check1 = info[i].condition;

        if (check1 != check2)
        {
            if (false == check1)
            {
                plist<<(&(info[i-1].DAC));
            }
            else
            {
                plist<<(&(info[i].DAC));

                PartIterpolation(plist);

                plist.clear();
            }
        }

        if (false == check1)
        {
            plist<<(&(info[i].DAC));
        }

        check2 = check1;
    } 
}

/**
 *
 *  @author  William
 *
 *  @date    2016/06/21
 *
 *  @fn      QValueList<int> TInterpolation::PartIterpolation(int start, int end, int number1, int number2)
 *
 *  @brief   ( do linearly )
 *
 *  @param   int start, int end, int number1, int number2
 *
 *  @return  ( QValueList<int> )
 *
 *  @note    none
 *
 */
void TInterpolation::PartIterpolation(QValueList<int*> plist)
{
    for( int i = 1; i < plist.count()-1; i++ )
    {
        // save to list
        *plist[i] = (double)(*(plist[plist.count()-1]) - *plist[0]) / (plist.count()-1) * i + *plist[0];
    }
}
