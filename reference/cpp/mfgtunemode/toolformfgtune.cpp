#include <qdir.h>
#include <qfile.h>
#include <qtimer.h>
#include <qfileinfo.h>
#include <qtextstream.h>

#include "toolformfgtune.h"

#include <stdlib.h> 
#include <stdio.h>
#include <time.h>

#include <stdlib.h>

/**
 *
 *  @author  William
 *
 *  @date    2016/06/22
 *
 *  @fn      void ToolForMfg::createMfgFolder()
 *
 *  @brief   ( check exist & make path )
 *
 *  @param   none
 *
 *  @return  none
 *
 *  @note    none
 *
 */
void ToolForMfg::createMfgFolder()
{
    QDir dir(MfgTunePath);

    if (!dir.exists()) 
    {
        dir.mkdir(MfgTunePath);
    }
}

 /**
 *
 *  @author  William
 *
 *  @date    2016/06/22
 *
 *  @fn      void ToolForMfg::WriteAntennaTuneValueToFile( QString str )
 *
 *  @brief   ( write to file )
 *
 *  @param   ( string )
 *
 *  @return  none
 *
 *  @note    none
 *
 */
void ToolForMfg::WriteAntennaTuneValueToFile( AntennaTuneInfoList info )
{
    u3log_debug("==== ToolForMfg::WriteAntennaTuneValueToFile ====" );

    // check path
    createMfgFolder();

    // get time
    QDate d = QDate::currentDate();
    QTime t = QTime::currentTime();
    QString timestr;
    timestr.sprintf("[%04d/%02d/%02d %02d:%02d:%02d]",d.year(),d.month(),d.day(),
        t.hour(),t.minute(),t.second());

    //AntennaTunePath = "/runpara/Applications/mfg_tune/AntennaDAC";
    QFile file(AntennaTunePath);

    // check open file
    if (!file.open( IO_WriteOnly ))
    {
        u3log_error("Can't Open File" );

        return;
    }

    // save to file
    QTextStream out(&file);  
    QString str;
    for( int j = 0; j < info.count(); j++ )
    {
        str.sprintf("%s:%02u %s:%u %s:%10.6f %s:%5d", "item" , info[j].item ,
                                                 "freq" , info[j].freq ,
                                                 "rssi" , info[j].rssi ,
                                                 "dac"  , info[j].DAC );
        
        out << timestr << " " << str <<"\r\n";
    }

    // close file
    file.close();

    u3log_debug("Write to File Success" );
}

 /**
 *
 *  @author  Alex
 *
 *  @date    2016/07/06
 *
 *  @fn      void ToolForMfg::Write13MHzTuneValueToFile( unsigned short dac )
 *
 *  @brief   ( write to file )
 *
 *  @param   ( result : result )
 *
 *  @return  none
 *
 *  @note    none
 *
 */
void ToolForMfg::Write13MHzTuneValueToFile( unsigned short dac )
{
    u3log_debug("==== ToolForMfg::Write13MHzTuneValueToFile ====" );

    // check path
    createMfgFolder();

    // get time
    QDate d = QDate::currentDate();
    QTime t = QTime::currentTime();
    QString timestr;
    timestr.sprintf("[%04d/%02d/%02d %02d:%02d:%02d]",d.year(),d.month(),d.day(),
        t.hour(),t.minute(),t.second());

    //AntennaTunePath = "/runpara/Applications/mfg_tune/AntennaDAC";
    QFile file(T13MHzTunePath);

    // check open file
    if (!file.open( IO_WriteOnly ))
    {
        u3log_error("Can't Open File" );

        return;
    }

    // save to file
    QTextStream out(&file);  
    QString str;
    str.sprintf("%d", dac);
        
    out << timestr << " " << str;
    // close file
    file.close();

    u3log_debug("Write to File Success" );
}

/**
 *
 *  @author  William
 *
 *  @date    2016/06/28
 *
 *  @fn      unsigned short TCalibrateData::crc_ccitt
 *
 *  @brief   ( get checksum value )
 *
 *  @param   unsigned short * ptr, int length
 *
 *  @return  checksum value
 *
 *  @note    none
 *
 */
unsigned short ToolForMfg::crc_ccitt( unsigned short * ptr, int length)
{    
    u3log_debug("==== ToolForMfg::crc_ccitt ====" );

    // calculate check sum
    unsigned short temp;
    unsigned short crc;
    int i, j;

    temp = 0;
    for ( i=0; i<length; i++ )
    {
        for ( j=7; j>=0; j-- )
        {
            if ( ( (temp >> 15) ^ ((*(ptr+i)) >> j) ) & 1 )
                temp = (temp << 1) ^ G_16;
            else temp = temp << 1;
        }
    }
    crc =(unsigned short) ( temp & 0xffff )^0xffff;

    u3log_debug("==== crc<%d> ====", crc);
    
    return ( crc );
}

bool ToolForMfg::WriteBERResultToFile(int frequency , int level , double BER , double AverBER,double RSSI)
{	
	
	FILE * fp = fopen( "/runpara/Applications/mfg_tune/BitErrorRate" , "a" );
	FILE * fp2 = fopen( "/tmp/BitErrorRate" , "a" ); 
	
	time_t ct = time(0);
    struct tm *p = localtime(&ct);
	
	if( fp != NULL )
	{
		u3log_error("write ber");
		fprintf( fp , "[%02d-%02d_%02d:%02d:%02d] %d %d %2.2f %2.2f %2.2f\n", 
		 p->tm_mon+1, p->tm_mday, p->tm_hour, p->tm_min, p->tm_sec, frequency, level, BER, AverBER ,RSSI);
		fprintf( fp2 , "[%02d-%02d_%02d:%02d:%02d] %d %d %2.2f %2.2f %2.2f\n", 
		 p->tm_mon+1, p->tm_mday, p->tm_hour, p->tm_min, p->tm_sec, frequency, level, BER, AverBER ,RSSI);
	}
	else
	{
		return false;	
	}
	
	fclose(fp);
	fclose(fp2);

	return true;
}

bool ToolForMfg::WriteRSSIResultToFile(int frequency , int level , double RSSI)
{
	FILE * fp = fopen( "/runpara/Applications/mfg_tune/RSSI" , "a" );
	FILE * fp2 = fopen( "/tmp/RSSI" , "a" );
	time_t ct = time(0);
    struct tm *p = localtime(&ct);
	
	if( fp != NULL )
	{
		u3log_error("write rssi");
		fprintf( fp , "[%02d-%02d_%02d:%02d:%02d] %d %d %2.2f\n", 
		 p->tm_mon+1, p->tm_mday, p->tm_hour, p->tm_min, p->tm_sec, frequency, level, RSSI );
		fprintf( fp2 , "[%02d-%02d_%02d:%02d:%02d] %d %d %2.2f\n", 
		 p->tm_mon+1, p->tm_mday, p->tm_hour, p->tm_min, p->tm_sec, frequency, level, RSSI );
	}
	else
	{
		return false;	
	}
	
	fclose(fp);
	fclose(fp2);
	
	
	return true;
}

bool ToolForMfg::WriteRSSIToFile(int RSSI,int errorPct,int BerS,int BerL)
{
	FILE * fp = fopen( "/runpara/Applications/mfg_tune/TEST_RSSI" , "a" );

	time_t ct = time(0);
    struct tm *p = localtime(&ct);
	
	if( fp != NULL )
	{
		u3log_error("write rssi");
		fprintf( fp , "[%02d-%02d_%02d:%02d:%02d] %d, %d, %d, %d\n", 
				 p->tm_mon+1, p->tm_mday, p->tm_hour, p->tm_min, p->tm_sec, RSSI,errorPct,BerS,BerL );
	}
	else
	{
		return false;	
	}
	
	fclose(fp);
	
	return true;
}
