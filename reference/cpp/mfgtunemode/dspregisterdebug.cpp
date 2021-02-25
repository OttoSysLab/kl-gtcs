#include <qtopia/businesslogic/globaldefine.h>
#include <qtopia/dspinterface/dspinterface.h>
#include <qtopia/qcopenvelope_qws.h>
#include <qtopia/businesslogic/recvtableconfigpacker.h>
#include <qtopia/transmitscheduler1/dsprfconfigurator.h>

#include "dspregisterdebug.h"

#include <qtopia/config.h>
#include <qdir.h>
#include <qfile.h>
#include <qfileinfo.h>
#include <qtimer.h>
#include <unistd.h>
#include <math.h>
#include <stdlib.h>
#include <qtextstream.h>
#include <qfileinfo.h>
#include <time.h>

#define U3_LOG_FILE_PATH            "/log/AppsLog/u3log.log.old"
#define SYSTEM_MESSAGE_FILE_PATH    "/log/SysLog/messages.0"
#define TEST_CONFIG_NAME            "DSPRegisterDebug"
#define TEST_GROUP_NAME             "DSPRegister"
#define TEST_GROUP_NAME2            "BACKUPLOG"
#define LOG_RSSI_FILE_PATH          "/userdate/RSSILOG"

TDSPRegisterDebug * TDSPRegisterDebug::m_instance = NULL;

TDSPRegisterDebug * TDSPRegisterDebug::instance()
{
    if (m_instance == NULL) {
        m_instance = new TDSPRegisterDebug();
    }

    return m_instance;
}

/**
 *
 *  @author  Alex
 *
 *  @date    2016/06/20
 *
 *  @fn      TDSPRegisterDebug::TDSPRegisterDebug(QObject *parent)
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
TDSPRegisterDebug::TDSPRegisterDebug(QObject *parent)
:QObject(parent)
{
	m_count = 0;
    m_TestEnable = false;
    s_fpR = NULL;
    GetTestConfigData();

    m_VolumeTimer = new QTimer(this);
    connect(m_VolumeTimer, SIGNAL(timeout()), this, SLOT(slt_RunTestTimeout()));

    QCopChannel *powerChannel = new QCopChannel(POWER_ONOFF_CH, this );
    connect(powerChannel, SIGNAL(received(const QCString&, const QByteArray&)), this,SLOT(slt_RunTestTimeout()) );  

	QCopChannel *time = new QCopChannel("dspdebug", this );
	connect(time, SIGNAL(received(const QCString&, const QByteArray&))
			, this, SLOT(slt_received(const QCString &, const QByteArray &))); 
	
    m_GetLogTimer = new QTimer(this);
    connect(m_GetLogTimer, SIGNAL(timeout()), this, SLOT(slt_PackLogDataToAdcFile()));
    m_AutoLogTimer = new QTimer(this);
    connect(m_AutoLogTimer, SIGNAL(timeout()), this, SLOT(slt_AutoLogBackup()));
	
    m_readRegisterTimer = new QTimer(this);
    connect(m_readRegisterTimer, SIGNAL(timeout()), this, SLOT(slt_readRegisterTimeout()));

    slt_RunTestTimeout();
	slt_AutoLogBackup();
    InitRegisterData();
    tryStartRegister();
    
    Check_RSSILOG_Path();
    
    QCopChannel *debug = new QCopChannel("G5/RSSILog", this);
    connect(debug, SIGNAL(received(const QCString&,const QByteArray&)),
        this, SLOT(slot_debugrssi(const QCString&, const QByteArray&))); 
        
    m_RSSILogTimer = new QTimer(this);
    connect(m_RSSILogTimer, SIGNAL(timeout()), this, SLOT(slt_RSSILogTimer()));
    m_RSSILogTimer->stop();   
    
    //set power flag
    TShareMemory().m_shm->powerSave.rssilog = false;              
}

/**
 *
 *  @author  Alex
 *
 *  @date    2016/06/20
 *
 *  @fn      TDSPRegisterDebug::~TDSPRegisterDebug()
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
TDSPRegisterDebug::~TDSPRegisterDebug()
{
}

void TDSPRegisterDebug::slt_received(const QCString& msg, const QByteArray& data)
{
	u3log_debug("======== TDSPRegisterDebug::slt_received ========");
	if(msg == "DumpTime(int)")
	{
		int time = 0;
		QDataStream stream(data,IO_ReadOnly);
		stream >> time;
		
		m_count++;
		writeToFile(m_count,time);
	}
}
/**
 *
 *  @author  Ming
 *
 *  @date    2016/08/12
 *
 *  @fn      void TDSPRegisterDebug::GetTestConfigData()
 *
 *  @brief   ( Read Config Data To Buffer )
 *
 *  @param
 *
 *  @return  none
 *
 *  @note    none
 *
 */
void TDSPRegisterDebug::GetTestConfigData()
{
    Config config(TEST_CONFIG_NAME);
    config.setGroup(TEST_GROUP_NAME);

    m_TestEnable = config.readBoolEntry("Enable",false);
    u3log_debug("======== m_TestEnable : %d ========" , m_TestEnable);
    m_TestInterval = config.readNumEntry("IntervalTime", 5);
    u3log_debug("======== m_TestInterval : %d ========" , m_TestInterval);
    m_TestValue = config.readNumEntry("Value", 50000);
    u3log_debug("======== m_TestValue : %d ========" , m_TestValue);
    m_TestDelayTime = config.readNumEntry("DelayTime", 30);
    u3log_debug("======== m_TestDelayTime : %d ========" , m_TestDelayTime);
    m_CurrentTestTime = m_TestInterval;
    m_IsRetryFunction = config.readBoolEntry("IsRetry",false);
    u3log_debug("======== m_IsRetryFunction : %d ========" , m_IsRetryFunction);
	
	
    config.setGroup(TEST_GROUP_NAME2);
	m_ATSLogEnable = config.readBoolEntry("Enable",false);
	u3log_debug("======== m_ATSLogEnable : %d ========" , m_ATSLogEnable);
	m_BackupTime = config.readNumEntry("BACKUPTime", 10);
	u3log_debug("======== m_BackupTime : %d ========" , m_BackupTime);
	m_DspLogEnable = config.readBoolEntry("DSPLogEnable",false);
	u3log_debug("======== m_DspLogEnable : %d ========" , m_DspLogEnable);
	m_AppsLogEnable = config.readBoolEntry("AppsLogEnable",false);
	u3log_debug("======== m_AppsLogEnable : %d ========" , m_AppsLogEnable);
	m_SysLogEnable = config.readBoolEntry("SysLogEnable",false);
	u3log_debug("======== m_SysLogEnable : %d ========" , m_SysLogEnable);
	m_ArmTraceLogEnable = config.readBoolEntry("ArmTraceLogEnable",false);
	u3log_debug("======== m_ArmTraceLogEnable : %d ========" , m_ArmTraceLogEnable);
}

/**
 *
 *  @author  Ming
 *
 *  @date    2016/08/12
 *
 *  @fn      void TDSPRegisterDebug::slt_RunTestTimeout()
 *
 *  @brief   ( Run Test Function )
 *
 *  @param
 *
 *  @return  none
 *
 *  @note    none
 *
 */
void TDSPRegisterDebug::slt_RunTestTimeout()
{
    u3log_debug("======== TDSPRegisterDebug::slt_RunTestTimeout ========");

    if( true == m_TestEnable )
    {
        u3log_debug("======== start Test Function ========");
        int vaule = GetVolumeTestVaule();
        u3log_debug("vaule<%d>", vaule);
        
        if (vaule >= 0)
        {
            if (vaule < m_TestValue)
            {
                u3log_error("get Volume Value<%d>", vaule);
        
                m_CurrentTestTime--;
        
                if (m_CurrentTestTime <= 0)
                {
                    m_CurrentTestTime = m_TestInterval;
                    // After N Get Value
                    StartGetLogTimer( m_TestDelayTime );
                    return;
                }
        
            }
            else
            {
                m_CurrentTestTime = m_TestInterval;
            }
        }
        
        CheckU3logHasOldFile();
        CheckSystemMessageHasOldFile();
        CompareOneHourTime();
        m_VolumeTimer->start(1000, true);
    }
}

/**
 *
 *  @author  Ming
 *
 *  @date    2016/08/12
 *
 *  @fn      void TDSPRegisterDebug::CompareOneHourTime()
 *
 *  @brief   ( Compare One Hour Time Pack Data )
 *
 *  @param
 *
 *  @return  none
 *
 *  @note    none
 *
 */
void TDSPRegisterDebug::CompareOneHourTime()
{
    QTime nowTime = QTime::currentTime();
        
    if( ( 0x00 == nowTime.minute() ) &&
        ( 0x00 == nowTime.second() ) )
    {
        slt_PackLogDataToAdcFile();
    }
}

/**
 *
 *  @author  Ming
 *
 *  @date    2016/08/12
 *
 *  @fn      void TDSPRegisterDebug::CheckU3logHasOldFile()
 *
 *  @brief   ( Check u3log old file to userdata path )
 *
 *  @param
 *
 *  @return  none
 *
 *  @note    none
 *
 */
void TDSPRegisterDebug::CheckU3logHasOldFile()
{
    if( IsFileExists( U3_LOG_FILE_PATH ) == true )
    {
        u3log_debug("======== Move %s ========" , U3_LOG_FILE_PATH );
        ::system("mkdir /userdata/ADC");
        ::system("mkdir /userdata/ADC/AppsLog");
        QString cmd = QString( "mv " ) + QString( U3_LOG_FILE_PATH ) + QString( " /userdata/ADC/AppsLog/" ) + GetDateTimeFileName();
        ::system(cmd.latin1());
    }
}

/**
 *
 *  @author  Ming
 *
 *  @date    2016/08/12
 *
 *  @fn      void TDSPRegisterDebug::CheckSystemMessageHasOldFile()
 *
 *  @brief   ( Check System message old file to userdata path )
 *
 *  @param
 *
 *  @return  none
 *
 *  @note    none
 *
 */
void TDSPRegisterDebug::CheckSystemMessageHasOldFile()
{
    if( IsFileExists( SYSTEM_MESSAGE_FILE_PATH ) == true )
    {
        u3log_debug("======== Move %s ========" , SYSTEM_MESSAGE_FILE_PATH);
        ::system("mkdir /userdata/ADC");
        ::system("mkdir /userdata/ADC/SysLog");
        QString cmd = QString( "mv " ) + QString( SYSTEM_MESSAGE_FILE_PATH ) + QString( " /userdata/ADC/SysLog/" ) + GetDateTimeFileName();
        ::system(cmd.latin1());
    }
}

/**
 *
 *  @author  Ming
 *
 *  @date    2016/08/12
 *
 *  @fn      QString TDSPRegisterDebug::GetDateTimeFileName()
 *
 *  @brief   ( Get Now Date & Time String  )
 *
 *  @param
 *
 *  @return  ( Now Date & Time String )
 *
 *  @note    none
 *
 */
QString TDSPRegisterDebug::GetDateTimeFileName()
{
    QDateTime now = QDateTime::currentDateTime();
    return QString::number( now.date().year() ) +
           QString::number( now.date().month() ).rightJustify (2,'0') +
           QString::number( now.date().day() ).rightJustify (2,'0') +
           "_" +
           QString::number( now.time().hour() ).rightJustify (2,'0') +
           QString::number( now.time().minute() ).rightJustify (2,'0');
}

/**
 *
 *  @author  Ming
 *
 *  @date    2016/08/12
 *
 *  @fn      bool TDSPRegisterDebug::IsFileExists( QString path )
 *
 *  @brief   ( Check file is Exists  )
 *
 *  @param
 *
 *  @return  ( true : has file / false : not file )
 *
 *  @note    none
 *
 */
bool TDSPRegisterDebug::IsFileExists( QString path )
{
    QFileInfo file( path );
    return file.exists();
}

/**
 *
 *  @author  Ming
 *
 *  @date    2016/08/12
 *
 *  @fn      int TDSPRegisterDebug::GetVolumeTestVaule()
 *
 *  @brief   ( Get Volume knob ADC value )
 *
 *  @param
 *
 *  @return  ( Volume knob ADC value )
 *
 *  @note    none
 *
 */
int TDSPRegisterDebug::GetVolumeTestVaule()
{
    u3log_debug("======== TDSPRegisterDebug::GetVolumeTestVaule ========");

    unsigned short regValue = 0;
    register_access_t reg;
    reg.reg_id = 2050;

    if (getDspRegister(&reg) != 0)
    {
        u3log_error("get DSP Register fail!");
        return -1;
    }
    else
    {
        regValue = reg.reg_data[1]*256+ reg.reg_data[0];
        u3log_debug("regValue[%d]",regValue);

        return (int)regValue;
    }
}

/**
 *
 *  @author  Ming
 *
 *  @date    2016/08/12
 *
 *  @fn      void TDSPRegisterDebug::StartGetLogTimer( int setSec )
 *
 *  @brief   ( Start Get Log Timer )
 *
 *  @param
 *
 *  @return  none
 *
 *  @note    none
 *
 */
void TDSPRegisterDebug::StartGetLogTimer( int setSec )
{
    u3log_debug("StartGetLogTimer:%d",setSec);
    m_GetLogTimer->start( setSec * 1000 , true);
}

/**
 *
 *  @author  Ming
 *
 *  @date    2016/08/12
 *
 *  @fn      void TDSPRegisterDebug::slt_PackLogDataToAdcFile()
 *
 *  @brief   ( Pack Log Data and Get DSP Dump Data )
 *
 *  @param
 *
 *  @return  none
 *
 *  @note    none
 *
 */
void TDSPRegisterDebug::slt_PackLogDataToAdcFile()
{
    u3log_debug("Start Log Data!");
    // Copy DSP Log
    ::system("mkdir /userdata/ADC");
    ::system("mkdir /userdata/ADC_PACK");
    QCopEnvelope e("DSPEngine/DumpBuffer", "dumpDSPBuffers()");
    // Delay 500 ms Copy Data
    QTimer::singleShot( 300 , this , SLOT(slt_CopyFileToAdcFile()) );

}

void TDSPRegisterDebug::slt_AutoLogBackup()
{
	if( true == m_ATSLogEnable )
	{
		u3log_debug("Start dump!");
		QCopEnvelope e("DSPEngine/DumpBuffer", "dumpDSPBuffers()");
		m_AutoLogTimer->start(m_BackupTime*1000, true);
	}
}

/**
 *
 *  @author  Ming
 *
 *  @date    2016/08/12
 *
 *  @fn      void TDSPRegisterDebug::slt_CopyFileToAdcFile()
 *
 *  @brief   ( Copy Pack Data To Pack Path )
 *
 *  @param
 *
 *  @return  none
 *
 *  @note    none
 *
 */
void TDSPRegisterDebug::slt_CopyFileToAdcFile()
{
    InitRegisterData();
    
    u3log_debug("Copy Log Data!");
    
    // Copy Register
    ::system("cp -rf /userdata/register /userdata/ADC/");
    
    //Copy armTraceLog
    ::system("cp /log/AppsLog/armTrace.log /userdata/ADC/");

    // Copy DSP Log
    ::system("cp -rf /log/dspdump /userdata/ADC/");
     
    
    // Delay 500 ms Copy Data
    QTimer::singleShot( 300 , this , SLOT(slt_CopyFileToPackFile()) );

}

/**
 *
 *  @author  Ming
 *
 *  @date    2016/08/12
 *
 *  @fn      void TDSPRegisterDebug::slt_CopyFileToAdcFile()
 *
 *  @brief   ( Pack Data & Remove tmp data )
 *
 *  @param
 *
 *  @return  none
 *
 *  @note    none
 *
 */
void TDSPRegisterDebug::slt_CopyFileToPackFile()
{
    ::system("rm -rf /log/dspdump/");
    ::system("rm /userdata/register/*");
    
    //PackData
    m_savePath = GetDateTimeFileName();
    u3log_debug("nowTimeString : %s",m_savePath.latin1());
 
    QString systemCmd = "mv /userdata/ADC /userdata/" + m_savePath;
    ::system(systemCmd.latin1());
        
    slt_PackDataFunc();
        
    if( true == m_IsRetryFunction )
    {
        u3log_debug("=== Retry! ===");
        slt_RunTestTimeout();
    }
    
    tryStartRegister();
}

/**
 *
 *  @author  Ming
 *
 *  @date    2016/08/12
 *
 *  @fn      void TDSPRegisterDebug::slt_PackDataFunc()
 *
 *  @brief   ( Create Script File And script to background )
 *
 *  @param
 *
 *  @return  none
 *
 *  @note    none
 *
 */
void TDSPRegisterDebug::slt_PackDataFunc()
{
    QString systemCmd = "tar -zpcv -f /userdata/ADC_PACK/" + 
                        m_savePath +
                        ".tar.gz /userdata/" +
                        m_savePath + 
                        "/ ; ";
    systemCmd += "rm -rf /userdata/" + m_savePath;
    u3log_debug("systemCmd : %s",systemCmd.latin1());
    
    QString filePath = "/userdata/" + m_savePath + ".sh";
    
    QFile file( filePath.latin1() );
    if ( file.open( IO_WriteOnly ) ) {
        QTextStream stream( &file );
        stream << systemCmd << "\n";
        file.close();
        systemCmd = "sh " + filePath + " &";
        ::system(systemCmd.latin1());
        systemCmd = "rm " + filePath;
        ::system(systemCmd.latin1());
    }
    
}

/**
 *
 *  @author  Ming
 *
 *  @date    2016/08/12
 *
 *  @fn      void TDSPRegisterDebug::InitRegisterData()
 *
 *  @brief   ( Init Register function )
 *
 *  @param
 *
 *  @return  none
 *
 *  @note    none
 *
 */
void TDSPRegisterDebug::InitRegisterData()
{
    if( s_fpR != NULL )
    {
        fclose(s_fpR);
    }


    
    s_fpR = NULL; 
    // Stop Get Register Data
    m_readRegisterTimer->stop();
}

/**
 *
 *  @author  Ming
 *
 *  @date    2016/08/12
 *
 *  @fn      void TDSPRegisterDebug::tryStartRegister()
 *
 *  @brief   ( Start Register Log )
 *
 *  @param
 *
 *  @return  none
 *
 *  @note    none
 *
 */
void TDSPRegisterDebug::tryStartRegister()
{
    printf("tryStartRegister=====\n");
    Config conf(TEST_CONFIG_NAME);

	conf.setGroup("Register");
    bool isRegEnable = conf.readBoolEntry("Enable",false);
	u3log_debug("===record reg enable:%d",isRegEnable);

    if (isRegEnable){
	    Q_UINT32 regCycleSeconds = conf.readNumEntry("CycleSeconds",1);
		u3log_debug("start recording reg,cycleSeconds:%d",regCycleSeconds);
	    QString idStr = conf.readEntry("RegisterIdList","");
		idStr = idStr.replace(QString("0x"),QString(""));
		idStr = idStr.replace(QString(" "),QString(""));
		u3log_debug("idStr[%s]",idStr.latin1());
		QStringList  idList = QStringList::split(',',idStr);
		m_registerIdList.clear();
		bool ok = false;
		for(int i = 0;i< idList.count();i++)
		{
			QString idItem =idList[i];
			u3log_debug("[%s]",idItem.latin1());
			//while((idItem.length() >1)&&(idItem[0]=='0'))
			//{
			//	idItem = idItem.remove(0,1);
			//}
			u3log_debug("[%s]",idItem.latin1());
		    int id = idItem.toInt(&ok,16);
			if(!ok)
			{	
				u3log_debug("idStr to int fail [%d] [%s]",id,idItem.latin1());
				m_registerIdList.clear();
				return;
			}
			m_registerIdList.append(id);
		}
		
        ::system("mkdir /userdata/register");
		writeRegisterLogTitle();
        m_readRegisterTimer->start(regCycleSeconds*1000);
    }
}

/**
 *
 *  @author  Ming
 *
 *  @date    2016/08/12
 *
 *  @fn      void TDSPRegisterDebug::writeRegisterLogTitle()
 *
 *  @brief   ( Write Register title Log )
 *
 *  @param
 *
 *  @return  none
 *
 *  @note    none
 *
 */
void TDSPRegisterDebug::writeRegisterLogTitle()
{
	if(m_registerIdList.count()==0) return;
	
    QDate d = QDate::currentDate();
    QTime t = QTime::currentTime();
	
	QString titleStr = "         Register Id :";

	for(int i=0; i< m_registerIdList.count();i++)
	{
		int id = m_registerIdList[i];

		QString idStr;
		idStr.sprintf(" [0x%04x],",id);
		titleStr += idStr;
	}

	titleStr +="\n";
	
	writeRegisterLog(titleStr);

}

/**
 *
 *  @author  Ming
 *
 *  @date    2016/08/12
 *
 *  @fn      void TDSPRegisterDebug::slt_readRegisterTimeout()
 *
 *  @brief   ( Read Register data )
 *
 *  @param
 *
 *  @return  none
 *
 *  @note    none
 *
 */
void TDSPRegisterDebug::slt_readRegisterTimeout()
{
	if(m_registerIdList.count()==0) return;
	
    QDate d = QDate::currentDate();
    QTime t = QTime::currentTime();
	

	QString valueStr ="";
    valueStr.sprintf("[%04d/%02d/%02d %02d:%02d:%02d] ",d.year(),d.month(),d.day(),
            t.hour(),t.minute(),t.second());

	for(int i=0; i< m_registerIdList.count();i++)
	{
		int id = m_registerIdList[i];
		int value = 0;
		
		if(!readRegisterValue(id, value)) return;

		QString vaStr;
	    vaStr.sprintf("  0x%04x ,",value);	


		valueStr += vaStr;
	}

	valueStr +="\n";

	
	writeRegisterLog(valueStr);
}

/**
 *
 *  @author  Ming
 *
 *  @date    2016/08/12
 *
 *  @fn      void TDSPRegisterDebug::slt_readRegisterTimeout()
 *
 *  @brief   ( write register data to register log )
 *
 *  @param
 *
 *  @return  none
 *
 *  @note    none
 *
 */
void TDSPRegisterDebug::writeRegisterLog(QString s)
{
	if (s_fpR == 0){
		QDate d = QDate::currentDate();
        QTime t = QTime::currentTime();
        QString fileName;
        fileName.sprintf("/userdata/register/readRegister_%04d%02d%02d_%02d%02d%02d",d.year(),d.month(),d.day(),
            t.hour(),t.minute(),t.second());


        s_fpR = fopen(fileName.latin1(),"w");
        if (!s_fpR){
            u3log_error("open rssi file fail:[%s]",fileName.latin1());
            return;
        }
        else{
            u3log_debug("open file:[%s] successfully!",fileName.latin1());
        }
    }

	fprintf(s_fpR,s.latin1());
    fflush(s_fpR);
}


/**
 *
 *  @author  Ming
 *
 *  @date    2016/08/12
 *
 *  @fn      bool TDSPRegisterDebug::readRegisterValue(int regId,int &value)
 *
 *  @brief   ( read register id Buffer value )
 *
 *  @param
 *
 *  @return  none
 *
 *  @note    none
 *
 */
bool TDSPRegisterDebug::readRegisterValue(int regId,int &value)
{
    struct register_access_t regAccess;
    memset(&regAccess, 0, sizeof(struct register_access_t));
    regAccess.reg_id = regId;


    int result = getDspRegister(&regAccess);
    if (result == 0) {
		value = 0;
		for(int i=0;i<regAccess.data_len;i++)
		{
			value += regAccess.reg_data[i] * pow(2,8*i);
		}
		
		u3log_debug("Reg ID 0x%x : %d", regId,value);
		return true;

    } else {
    	u3log_error("read [0x%04x] Register fail",regId);
        return false;
    }

}
void TDSPRegisterDebug::writeToFile(int Count,int Time)
{
	FILE * fp = fopen( "/runpara/Applications/mfg_tune/TEST_LOG" , "a" );
	
	time_t ct = time(0);
    struct tm *p = localtime(&ct);
	
	if( fp != NULL )
	{
		u3log_error("write count and time ");
		fprintf( fp , "[%02d-%02d_%02d:%02d:%02d] %d, %d\n", 
				 p->tm_mon+1, p->tm_mday, p->tm_hour, p->tm_min, p->tm_sec, Count,Time);
	}
	fclose(fp);
}
/**
 *
 *  @author  Kay 
 *
 *  @date    2017/09/25
 *
 *  @fn      void TDSPRegisterDebug::Check_RSSILOG_Path()
 *
 *  @brief   ( Check RSSILOG Path )
 *
 *  @param
 *
 *  @return  none
 *
 *  @note    none
 *
 */
void TDSPRegisterDebug::Check_RSSILOG_Path()
{
     u3log_debug("======== Check_RSSILOG_Path ========"  );
    if( IsFileExists( LOG_RSSI_FILE_PATH ) == false )
    {
        u3log_debug("======== mkdir %s ========" , LOG_RSSI_FILE_PATH );
        ::system("mkdir /userdata/RSSILOG");

    }
}


/**
 *
 *  @author  Kay
 *
 *  @date    2017/09/25
 *
 *  @fn      void TDSPRegisterDebug::writeRSSILog()
 *
 *  @brief   ( write register data to register log )
 *
 *  @param
 *
 *  @return  none
 *
 *  @note    none
 *
 */
void TDSPRegisterDebug::writeRSSILog()
{
	//if (s_fpR == 0){
	QDate d = QDate::currentDate();
    QTime t = QTime::currentTime();
        
    QString fileName;
    fileName.sprintf("/userdata/RSSILOG/rssilog_%04d%02d%02d",d.year(),d.month(),d.day());    
    //get time
	QString valueStr ="";
    valueStr.sprintf("[%04d/%02d/%02d %02d:%02d:%02d:%03d] ",d.year(),d.month(),d.day(),
            t.hour(),t.minute(),t.second(),t.msec() );
    u3log_debug("valueStr[%s]",valueStr.latin1());
    //get txFreq
    TChannelBaseInfo chBaseInfo;
	TContactBoard::getChannelBaseInfo(TBoard::lockedChannel(),chBaseInfo);            
	QString ferqStr ="";
    ferqStr.sprintf("rxfreq[%d]",chBaseInfo.rxFreq );
    u3log_debug("ferqStr[%s]",ferqStr.latin1());
    QString rssiStr ="";
     //get RSSI
	struct RSSIMSRRegister reg;
	getMsrRssi(&reg);
    rssiStr.sprintf("rssi[%d]",(int)reg.RSSILevel);
    u3log_debug("rssiStr[%s]",rssiStr.latin1());
    QString dbm ="";
    dbm.sprintf("dbm[%d]",(int)(reg.RSSILevel/128));
    u3log_debug("dbm[%s]",dbm.latin1());   
    //Write data to file
    QString cmd = QString( "echo " )+QString(valueStr +","+ferqStr+","+rssiStr+","+dbm) + " >> " + QString(fileName.latin1());
    ::system(cmd.latin1());
    u3log_debug("cmd:[%s] ",cmd.latin1());       


}
/**
 *
 *  @author  Kay
 *
 *  @date    2017/09/26
 *
 *  @fn      void TDSPRegisterDebug::StartRssiLog()
 *
 *  @brief   ( write Start Rssi Log )
 *
 *  @param
 *
 *  @return  none
 *
 *  @note    none
 *
 */
void TDSPRegisterDebug::StartRssiLog(int time)
{
   u3log_debug(" TReceiveconditionswitch::StartRssiLog"); 
   m_RSSILogTimer->start(time, false); 
   //set power flag tren ,no power save
   TShareMemory().m_shm->powerSave.rssilog = true;  
   //writeRSSILog();
}

/**
 *
 *  @author  Kay
 *
 *  @date    2017/09/26
 *
 *  @fn      void TDSPRegisterDebug::StopRssiLog()
 *
 *  @brief   ( write Stop Rssi Log )
 *
 *  @param
 *
 *  @return  none
 *
 *  @note    none
 *
 */
void TDSPRegisterDebug::StopRssiLog()
{
   u3log_debug(" TReceiveconditionswitch::StopRssiLog"); 
   m_RSSILogTimer->stop();
   //set power flag false ,in power save
   TShareMemory().m_shm->powerSave.rssilog = false;   
    
   //get Zone and ch
   TZoneKnobInfo zk = TBoard::getLockedZoneKnob(true); 
   TZoneKnobId zoneKnobId = zk.zoneId*100+zk.knobId; 
      
   QValueList<U3SignalingType> protocolList; 
   TContactBoard::getListenProtocolList(protocolList, zoneKnobId);
   u3log_debug("ch Protocol~~~[%d]",protocolList[0]);
   
     TChannelRFInfo baseInfo;
    TBusinessGlobal::getCurChannelInfo(TBoard::lockedChannel(), baseInfo);
	u3log_debug("baseInfo.rxFreq.toInt()[%d]",baseInfo.rxFreq.toInt());  
      
   QCopEnvelope e("G5/OTACH", "RSSILOG()");
   e<<(int)baseInfo.rxFreq.toInt() <<(int)protocolList[0]<<(int)4;    
    
    
}

/**
 *
 *  @author  Kay
 *
 *  @date    2017/09/26
 *
 *  @fn      void TDSPRegisterDebug::StartRssiLog()
 *
 *  @brief   ( write Start Rssi Log )
 *
 *  @param
 *
 *  @return  none
 *
 *  @note    none
 *
 */
void TDSPRegisterDebug::slt_RSSILogTimer()
{
    u3log_debug("======== TDSPRegisterDebug::slt_testTimer ========");
    writeRSSILog();
     
} 

void TDSPRegisterDebug::slot_debugrssi(const QCString& msg, const QByteArray& data)
{
    u3log_debug(" TReceiveconditionswitch::slot_debug");
  
  if( msg == "start()")
  {    
     StartRssiLog(); 
     
  }
  else if(msg == "stop()")
  {
     StopRssiLog();

  } 
  else if(msg == "back()")
  {
     TBoard::setIsMFGTuneMode(true);
     //Back to old zone
     QCopEnvelope e("G4/MFGTune", "MFGTuneModeStop()");
  }
}    
   
