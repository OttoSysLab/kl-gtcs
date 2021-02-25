#include <stdlib.h>
#include <stdio.h>
#include <qfile.h>
#include <iostream>
#include <fstream>
#include <sys/mount.h>
#include <linux/fs.h>

#include <dspadapter/caldata_access_interface.h>
#include <qtopia/businesslogic/contactboard.h>
#include <qtopia/businesslogic/informationdbaccess.h>
#include <caldb/rfiudefs.h>

#include "toolformfgtune.h"

#include "calibratedata.h"


/**
 *
 *  @author  William
 *
 *  @date    2016/06/24
 *
 *  @fn      TCalibrateData::TCalibrateData(QObject *parent)
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
TCalibrateData::TCalibrateData(QObject *parent)
:QObject(parent)
{
}

/**
 *
 *  @author  William
 *
 *  @date    2016/06/24
 *
 *  @fn      TCalibrateData::~TCalibrateData()
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
TCalibrateData::~TCalibrateData()
{
}

/**
 *
 *  @author  Alex
 *
 *  @date    2016/07/06
 *
 *  @fn      bool TCalibrateData::Write13MHzResult(unsigned short result)
 *
 *  @brief   ( write to block )
 *
 *  @param   ( dac : result )
 *
 *  @return  success
 *
 *  @note    none
 *
 */
bool TCalibrateData::Write13MHzResult(unsigned short dac)
{
    u3log_debug("==== TCalibrateData::Write13MHzResult ====" );

    // unlock
    mount("/dev/calpar", "/usr/calibration", "yaffs2", MS_SILENT | MS_REMOUNT, "no-verify");

    // write driver
    u3log_debug("Start Write CalBlock");
    int ret = 0;

    ret = writeCalBlock(AFC_DAC_CAL, FIELD_CAL, (unsigned char *)&dac, sizeof(unsigned short), TBusinessGlobal::getBoardBandType());

    if ( ret != 0 )
    {
        u3log_error("Set FIELD_CAL Fail <%d>", ret);
    }
    else
    {
        u3log_debug("Set FIELD_CAL Success");
    }
    
    ret = writeCalBlock(AFC_DAC_CAL, MFG_CAL, (unsigned char *)&dac, sizeof(unsigned short), TBusinessGlobal::getBoardBandType());

    if ( ret != 0 )
    {
        u3log_error("Set MFG_CAL Fail <%d>", ret);
    }
    else
    {
        u3log_debug("Set MFG_CAL Success");
    }

    // lock
    mount("/dev/calpar", "/usr/calibration", "yaffs2", MS_SILENT | MS_REMOUNT | MS_RDONLY, "no-verify");

    return true;
}

/**
 *
 *  @author  Alex
 *
 *  @date    2016/06/20
 *
 *  @fn      void TCalibrateData::WriteCalibrateData(AntennaTuneInfoList info)
 *
 *  @brief   ( write to calibration )
 *
 *  @param   ( info : information )
 *
 *  @return  success
 *
 *  @note    none
 *
 */
bool TCalibrateData::WriteCalibrateData( AntennaTuneInfoList info )
{
    u3log_debug("TCalibrateData::WriteCalibrateData"); 

    MFGControlChFreqInfo tmp_mfgControlChInfo;
    //read Applications/mfg_tune/mfg.db data to tmp_mfgControlChInfo 
    TBusinessGlobal::getMFGControlChInfo( tmp_mfgControlChInfo );
    m_RfcalLen = tmp_mfgControlChInfo.RfcalLen;
    u3log_debug("==== RfcalLen : %d ====", m_RfcalLen);

    QByteArray byte(m_RfcalLen);

    // get calibration data
    GetCalibraionData( byte );

    // set data to calibration data
    SetAntennaTuneData(info, byte);

    // set data to calibration data
    SetCheckSum(byte);

    // write data to calibration
    SetCalibrationData(byte);

    return true;
}

/**
 *
 *  @author  William
 *
 *  @date    2016/06/28
 *
 *  @fn      void TCalibrateData::SetCheckSum
 *
 *  @brief   ( take out )
 *
 *  @param   QByteArray &caldata
 *
 *  @return  caldata
 *
 *  @note    none
 *
 */
void TCalibrateData::GetCalibraionData( QByteArray &caldata )
{
    u3log_debug("==== void TCalibrateData::GetCalibraionData ====" );

    char *filename = new char[430];
    memset(filename,0,sizeof(filename));

    if (get_default_rfcal_db(filename, 128) == 0)
    {
        QString qfile(filename);

        // SAW Filter HW
        QString ModelNo = TInformationDBAccess::getDeviceModelNumber().left(3);
        QString rfBoard = TBusinessGlobal::getRFBoardVersion();

        if (TBusinessGlobal::checkSAWFilter(ModelNo) && (0 <=  rfBoard.find("0PB4") || 0 <= rfBoard.find("NPB4")))
        {
            qfile.insert(qfile.find("/rf/") + 3, "saw");
        }

        // Read file
        QFile srcFile(qfile);
        if (!srcFile.open(IO_ReadOnly))
        {
            u3log_error("==== Read file error ====" );

            delete []filename;

            return;
        }

        // Set to QByteArray
        caldata = srcFile.readAll();

        // Close file
        srcFile.close();
    }
    else
    {
        u3log_error("==== Read file name error ====" ); 
    }

    delete []filename;
}

/**
 *
 *  @author  William
 *
 *  @date    2016/06/28
 *
 *  @fn      void TCalibrateData::SetCheckSum
 *
 *  @brief   ( change assignation QByteArray value )
 *
 *  @param   AntennaTuneInfoList list, QByteArray &caldata
 *
 *  @return  caldata
 *
 *  @note    none
 *
 */
void TCalibrateData::SetAntennaTuneData(AntennaTuneInfoList list, QByteArray &caldata)
{
    u3log_debug("==== TCalibrateData::SetAntennaTuneData ====");

    // list[i].dac change to assignation QByteArray value
    for( int i = 0 ; i < list.count(); i++ )
    {
        u3log_debug("==== list[%d].DAC = %d ====" ,i,list[i].DAC );
        // first localtion is 3c 0A
        caldata[278+(i*2)  ] =(char)( list[i].DAC      & 0xFF);
        caldata[278+(i*2)+1] =(char)((list[i].DAC >>8) & 0xFF);
    }
}

/**
 *
 *  @author  William
 *
 *  @date    2016/06/28
 *
 *  @fn      void TCalibrateData::SetCheckSum
 *
 *  @brief   ( Set caldata add checksum )
 *
 *  @param   QByteArray &caldata
 *
 *  @return  caldata
 *
 *  @note    none
 *
 */
void TCalibrateData::SetCheckSum(QByteArray &caldata)
{
    u3log_debug("==== void TCalibrateData::SetCheckSum ====" );

    caldata[4] = 0;
    caldata[5] = 0;
    unsigned short *data = (unsigned short *) calloc( m_RfcalLen,  sizeof(short) );
    unsigned short crc;

    for( int i = 0 ; i < caldata.count(); i++ )
    {
        data[i] = caldata[i];
    }
    // get checksum
    crc = ToolForMfg::crc_ccitt(data,m_RfcalLen);
    
    caldata[4] = ( crc     & 0xFF);
    caldata[5] = ((crc >>8)& 0xFF);
    
    free(data);
}

/**
 *
 *  @author  William
 *
 *  @date    2016/06/28
 *
 *  @fn      void TCalibrateData::SetCalibrationData(QByteArray &caldata)
 *
 *  @brief   ( write to back )
 *
 *  @param   QByteArray &caldata
 *
 *  @return  caldata
 *
 *  @note    none
 *
 */
void TCalibrateData::SetCalibrationData(QByteArray &caldata)
{
    u3log_debug("==== TCalibrateData::SetCalibrationData ====" );

    // unlock
    mount("/dev/calpar", "/usr/calibration", "yaffs2", MS_SILENT | MS_REMOUNT, "no-verify");

    uint32_t *csize = new uint32_t;
    uint8_t *data = new uint8_t[m_RfcalLen];

    memcpy (data, caldata.data(), m_RfcalLen);

    if (writeCalImg(RF_CAL_DB, data, 0, caldata.count(), csize) != 0)
    {
        u3log_error("SetCalibrationData Fail");
    }
    else
    {
        u3log_debug("SetCalibrationData Success");
    }

    // lock
    mount("/dev/calpar", "/usr/calibration", "yaffs2", MS_SILENT | MS_REMOUNT | MS_RDONLY, "no-verify");
    
    delete csize;
    delete[] data;
}

