/*=======================================================================================
 Program Nane  	: gtcs_tlg_decoder.c
 Subject 		: SARM Serial Port Communication Driver Process
---------------------------------------------------------------------------------------
 Compiler/IDE  	: gcc 4.6.3
 Library       	:
 Commands      	: Geany Build Command
  				: Compile : gcc -Wall -I"../inc" -c "%f"
  				: Buid 	  : gcc -Wall -o "%e" "%f" "sarm2pcb.c" "pcb2sarm.c" "../lib/sarmdb.c" "../lib/sarmcomm.c" "../lib/sarmutility.c" -I"../inc" -l"sqlite3" -l"X11" `pkg-config --cflags --libs gtk+-3.0` -export-dynamic
 Programmer    	: Otto Chang
 Date	       	: 2019/08/06
=======================================================================================*/
#include "gtcsdatabase.h"
#include <sqlite3.h>

/******************************************************************************************
 *
 *  @author  Otto
 *
 *  @date    2021/02/04
 *
 *  @fn      Sqlite3Manager::Sqlite3Manager()
 *
 *  @brief   Constructor.
 *
 *  @param   none
 *
 *  @return  none
 *
 *  @note    none
 *
 *******************************************************************************************/
Sqlite3Manager::Sqlite3Manager()
{}
/******************************************************************************************
 *
 *  @author  Otto
 *
 *  @date    2021/02/04
 *
 *  @fn      Sqlite3Manager::~Sqlite3Manager()
 *
 *  @brief   Sqlite3Manager Distructer
 *
 *  @param   none
 *
 *  @return  none
 *
 *  @note    none
 *
 *******************************************************************************************/
Sqlite3Manager::~Sqlite3Manager()
{}
/******************************************************************************************
 *
 *  @author  Otto
 *
 *  @date    2021/02/04
 *
 *  @fn      Sqlite3Manager::UpdateDatabase(std::string db_Path,std::string table,std::string sqlcmd)
 *
 *  @brief   Write data to sqlite database.
 *
 *  @param   string db_Path
 * 
 *  @param   string table
 * 
 *  @param   string sqlcmd
 *
 *  @return  bool
 *
 *  @note    none
 *
 *******************************************************************************************/
bool Sqlite3Manager::UpdateDatabase(std::string db_Path,std::string table,std::string sqlcmd)
{
    // Initial value.
    sqlite3 *db;
    sqlite3_stmt *stmt;
    int rc;
    // Open database.
    rc = sqlite3_open(db_Path.c_str(),&db);
    if (rc)
    {
        std::cout<<"Can't open database : "<< sqlite3_errmsg(db) <<std::endl;
        return false;
    }
    // std::cout << "sqlcmd = " << sqlcmd <<std::endl;
    // updata database.
    rc = sqlite3_prepare_v2(db,sqlcmd.c_str(),-1,&stmt,NULL);
    if (rc != SQLITE_OK)
    {
        std::cout<<"Write SQL error:"<<sqlite3_errmsg(db)<<std::endl;
        sqlite3_finalize(stmt);
        return false;
    }

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_ROW && rc != SQLITE_DONE)
    {
        std::cout<<"Write SQL error:"<<sqlite3_errmsg(db)<<std::endl;
        sqlite3_finalize(stmt);
        return false;
    }

    // Finialize process.
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return true;
}
/******************************************************************************************
 *
 *  @author  Otto
 *
 *  @date    2021/02/04
 *
 *  @fn      Sqlite3Manager::ReadDatabase(std::string db_Path, std::string table,std::string *ptr)
 *
 *  @brief   Read data from sqlite database.
 *
 *  @param   string db_Path
 * 
 *  @param   string table
 * 
 *  @param   string *ptr
 *
 *  @return  bool
 *
 *  @note    none
 *
 *******************************************************************************************/
bool Sqlite3Manager::ReadDatabase(std::string db_Path, std::string table,std::string *ptr)
{
    // Initial sql cmd.
    std::string sqlcmd = "SELECT * from "+ table ;
    // Initial value.
    sqlite3 *db;
    sqlite3_stmt *stmt;
    int rc;
    // Open database.
    rc = sqlite3_open(db_Path.c_str(),&db);
    if (rc)
    {
        std::cout<<"Can't open database : "<< sqlite3_errmsg(db) <<std::endl;
        return false;
    }
    // Send SQL statement to db.
    rc = sqlite3_prepare_v2(db,sqlcmd.c_str(),-1,&stmt,NULL);
    if (rc != SQLITE_OK)
    {
        std::cout<<"Read SQL error:"<<sqlite3_errmsg(db)<<std::endl;
        sqlite3_finalize(stmt);
        return false;
    }
    // Excute step.
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_ROW && rc != SQLITE_DONE)
    {
        // string errmsg(sqlite3_errmsg(db));
        std::cout<<" Read SQL error:"<<sqlite3_errmsg(db)<<std::endl;
        sqlite3_finalize(stmt);
        return false;
    }
    if (rc == SQLITE_DONE) {
        sqlite3_finalize(stmt);
        std::cout<<"customer not found"<<std::endl;
        return false;
    }

    int index = 0;
    *ptr = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, index)));

    // Get data from sqlite pointer.
    while(true)
    {
        ptr = (std::string *)(void *)(ptr+1);
        if (*ptr != "\n\r")
        {
            index++;
            if (sqlite3_column_text(stmt, index)==NULL) // If get data == null,break the while loop.
            {
                break;
            }
            else
            {
                *ptr = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, index)));
            }
        }
        else
        {
            break;
        }
    }
    // Close sqlite3.
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return true;
}
/******************************************************************************************
 *
 *  @author  Otto
 *
 *  @date    2021/02/25
 *
 *  @fn      Sqlite3Manager::GetDatabaseTableDefaultValue(std::string &tablename, 
                                                        std::vector<std::string> &columnnames,
                                                        std::map<std::string,std::string> &coulunmtypes)
 *
 *  @brief   Read data from sqlite database.
 *
 *  @param   std::string &tablename
 * 
 *  @param   std::vector<std::string> &columnnames
 * 
 *  @param   std::map<std::string,std::string> &coulunmtypes
 *
 *  @return  std::string
 *
 *  @note    none
 *
 *******************************************************************************************/
std::string Sqlite3Manager::GetDatabaseTableDefaultValue(std::string &tablename, 
                                                        std::vector<std::string> &columnnames, 
                                                        std::map<std::string,std::string> &coulunmtypes)
{
    std::string result = "create table if not exists " + tablename +"(";

    int count = columnnames.size();
    
    #if defined(_DEBUG_MODE_)
    std::cout << "count = "<< std::to_string(count)<< std::endl;
    #endif
      
    for (int i = 0; i < count; i++)
    {
        
        #if defined(_DEBUG_MODE_)
        std::cout << "coulunmtypes[columnnames[i]] = "<< coulunmtypes[columnnames[i]]<< std::endl;
        #endif

        if(coulunmtypes[columnnames[i]]== "INTEGER")            // Colunm type = INTEGER.
        {
            result += columnnames[i] + " "+ coulunmtypes[columnnames[i]] + " default '0'";
        }
        else if (coulunmtypes[columnnames[i]]== "REAL")         // Colunm type = REAL.
        {
            result += columnnames[i] + " "+ coulunmtypes[columnnames[i]] + " default '0.0'";
        }
        else if (coulunmtypes[columnnames[i]]== "TEXT")         // Colunm type = TEXT.
        {
            result += columnnames[i] + " "+ coulunmtypes[columnnames[i]] +" default '-'" ;
        }
        else if ( coulunmtypes[columnnames[i]]== "DATETIME")    // Colunm type = TEXT.
        {
            result += columnnames[i] + " "+ coulunmtypes[columnnames[i]] +" default 'yyyy:mm:dd'" ;
        }
        else
        {
            #if defined(_DEBUG_MODE_)
            std::cout << "Fuck you !!" << std::endl;
            #endif
            return "error";
        }

        if (i!=(count-1))
        {
            result += ",";
        }
    }    
    result += ");";
    return result;
}
/******************************************************************************************
 *
 *  @author  Otto
 *
 *  @date    2021/02/04
 *
 *  @fn      Sqlite3Manager::GetDatabaseInsertSqlCommand(std::string &tablename,
                                                        std::vector<std::string> &colunmnames,
                                                        std::map<std::string,std::string> &colunmdatas)
 *
 *  @brief   GtcsTcsDatabase Constructor
 *
 *  @param   std::string &tablename
 * 
 *  @param   std::vector<std::string> &colunmnames
 * 
 *  @param   std::map<std::string,std::string> &colunmtypes
 * 
 *  @param   std::map<std::string,std::string> &colunmdatas
 *
 *  @return  none
 *
 *  @note    none
 *
 *******************************************************************************************/
std::string Sqlite3Manager::GetDatabaseInsertSqlCommand(std::string &tablename,
                                                        std::vector<std::string> &colunmnames,
                                                        std::map<std::string,std::string> &colunmtypes,
                                                        std::map<std::string,std::string> &colunmdatas)
{
    // Counter loop size
    int count = colunmnames.size();
    // Initial value.
    std::string result = "insert into " + tablename + "(";
    // 
    for (int i = 0; i < count; i++)
    {
        result += colunmnames[i];
        // Assign "," aftern data.
        if (i!=(count-1))
        {
            result += ",";
        }
    }
    result+=") values (";

    // Combine sql command.
    for (int i = 0; i < count; i++)
    {
        if (colunmtypes[colunmnames[i]]=="TEXT")
        {
            result += "'"+colunmdatas[colunmnames[i]]+"'";
        }
        else if (colunmtypes[colunmnames[i]]=="DATETIME")
        {
            result += "'"+colunmdatas[colunmnames[i]]+"'";
        }
        else
        {
            result += colunmdatas[colunmnames[i]];
        }        
        // Assign "," aftern data.
        if (i!=(count-1))
        {
            result += ",";
        }
    }
    result+=")";
    return result; 
}
/******************************************************************************************
 *
 *  @author  Otto
 *
 *  @date    2021/02/04
 *
 *  @fn      GtcsTcsDatabase::GtcsTcsDatabase(std::string Path)
 *
 *  @brief   GtcsTcsDatabase Constructor
 *
 *  @param   string Path
 *
 *  @return  none
 *
 *  @note    none
 *
 *******************************************************************************************/
GtcsTcsDatabase::GtcsTcsDatabase(std::string Path)
{
    dbPath = Path;
}
/******************************************************************************************
 *
 *  @author  Otto
 *
 *  @date    2021/02/04
 *
 *  @fn      GtcsTcsDatabase::~GtcsTcsDatabase()
 *
 *  @brief   GtcsTcsDatabase Distructor.
 *
 *  @param   none
 *
 *  @return  none
 *
 *  @note    none
 *
 *******************************************************************************************/
GtcsTcsDatabase::~GtcsTcsDatabase()
{}
/******************************************************************************************
 *
 *  @author  Otto
 *
 *  @date    2021/02/04
 *
 *  @fn      GtcsTcsDatabase::ReadDatabaseBasicData(GtcsTcsDatabaseBasicInfo &dbstruct)
 *
 *  @brief   ( Constructivist )
 *
 *  @param   GtcsTcsDatabaseBasicInfo &dbstruct
 *
 *  @return  bool
 *
 *  @note    none
 *
 *******************************************************************************************/
bool GtcsTcsDatabase::ReadDatabaseBasicData(GtcsTcsDatabaseBasicInfo &dbstruct)
{
    // Initial sqlcmd.
    std::string sqlcmd = "SELECT * from basic where rowid = 1;";
    // Initial value.
    sqlite3 *db;
    sqlite3_stmt *stmt;
    int rc;
    // Open database.
    rc = sqlite3_open(dbPath.c_str(),&db);
    if (rc)
    {
        std::cout<<"Can't open database : "<< sqlite3_errmsg(db) <<std::endl;
        return false;
    }
    // Send SQL statement to db.
    rc = sqlite3_prepare_v2(db,sqlcmd.c_str(),-1,&stmt,NULL);
    if (rc != SQLITE_OK)
    {
        std::cout<<"Read SQL error:"<<sqlite3_errmsg(db)<<std::endl;
        sqlite3_finalize(stmt);
        return false;
    }
    // Excute step.
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_ROW && rc != SQLITE_DONE)
    {
        // string errmsg(sqlite3_errmsg(db));
        std::cout<<" Read SQL error:"<<sqlite3_errmsg(db)<<std::endl;
        sqlite3_finalize(stmt);
        return false;
    }
    if (rc == SQLITE_DONE) {
        sqlite3_finalize(stmt);
        std::cout<<"customer not found"<<std::endl;
        return false;
    }

    // Assign dat to bsic struct.
    int columnname_size  = dbstruct.columnnames.size();    
    for (int i = 0; i < columnname_size; i++)
    {
        if (sqlite3_column_text(stmt, i)==NULL) // If get data == null,break the while loop.
        {
            return false;
        } 
        else
        {
            dbstruct.data[dbstruct.columnnames[i]] = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, i)));
        }
    }
    // Close sqlite3.
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return true;
}
/******************************************************************************************
 *
 *  @author  Otto
 *
 *  @date    2021/02/04
 *
 *  @fn      GtcsTcsDatabase::UpdateDatabaseBasicData(GtcsTcsDatabaseBasicInfo &dbstruct)
 *
 *  @brief   ( Constructivist )
 *
 *  @param   GtcsTcsDatabaseBasicInfo &dbstruct
 *
 *  @return  bool
 *
 *  @note    none
 *
 *******************************************************************************************/
bool GtcsTcsDatabase::UpdateDatabaseBasicData(GtcsTcsDatabaseBasicInfo &dbstruct)
{
    // Initial sql command.
    std::string sqlcmd = "update " + dbstruct.dbtablename + " set ";
    int columnnames_size  = dbstruct.columnnames.size();
    for (int i = 0; i < columnnames_size; i++)
    {
        if (dbstruct.type[dbstruct.columnnames[i]]!="TEXT")
        {
            sqlcmd += dbstruct.columnnames[i] + " = " + dbstruct.data[dbstruct.columnnames[i]] + ",";   
        }   
        else
        {
            sqlcmd += dbstruct.columnnames[i] + " = " + "'" + dbstruct.data[dbstruct.columnnames[i]] + "'" +",";   
        }
        // Test 
        // std::cout << "sqlcmd " << dbstruct.columnnames[i] << " = " << dbstruct.data[dbstruct.columnnames[i]] <<std::endl;
    }
    sqlcmd = sqlcmd.replace(sqlcmd.end()-1,sqlcmd.end()," ");
    sqlcmd += "where rowid = 1;";
    
    // Initial value.
    sqlite3 *db;
    sqlite3_stmt *stmt;
    int rc;
    // Open database.
    rc = sqlite3_open(dbPath.c_str(),&db);
    if (rc)
    {
        std::cout<<"Can't open database : "<< sqlite3_errmsg(db) <<std::endl;
        return false;
    }
    
    // std::cout << "sqlcmd = " << sqlcmd <<std::endl;

    // updata database.
    rc = sqlite3_prepare_v2(db,sqlcmd.c_str(),-1,&stmt,NULL);
    if (rc != SQLITE_OK)
    {
        std::cout<<"Write SQL error:"<<sqlite3_errmsg(db)<<std::endl;
        sqlite3_finalize(stmt);
        return false;
    }

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_ROW && rc != SQLITE_DONE)
    {
        std::cout<<"Write SQL error:"<<sqlite3_errmsg(db)<<std::endl;
        sqlite3_finalize(stmt);
        return false;
    }

    // Finialize process.
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return true;
}
/******************************************************************************************
 *
 *  @author  Otto
 *
 *  @date    2021/02/04
 *
 *  @fn      GtcsTcsDatabase::ReadDatabaseJobData(GtcsTcsDatabaseJobInfo &dbstruct,int jobid)
 *
 *  @brief   ( Constructivist )
 *
 *  @param   GtcsTcsDatabaseJobInfo &dbstruct
 * 
 *  @param   int jobid
 *
 *  @return  none
 *
 *  @note    none
 *
 *******************************************************************************************/
bool GtcsTcsDatabase::ReadDatabaseJobData(GtcsTcsDatabaseJobInfo &dbstruct,int jobid)
{
    // Initial sqlcmd.
    std::string sqlcmd = "SELECT * from ";
    sqlcmd += dbstruct.dbtablename;
    sqlcmd += " ";
    sqlcmd += " where job_id=";
    sqlcmd += std::to_string(jobid);
    sqlcmd += ";";
    
    // Initial value.
    sqlite3 *db;
    sqlite3_stmt *stmt;
    int rc;
    // Open database.
    rc = sqlite3_open(dbPath.c_str(),&db);
    if (rc)
    {
        std::cout<<"Can't open database : "<< sqlite3_errmsg(db) <<std::endl;
        return false;
    }
    // Send SQL statement to db.
    rc = sqlite3_prepare_v2(db,sqlcmd.c_str(),-1,&stmt,NULL);
    if (rc != SQLITE_OK)
    {
        std::cout<<"Read SQL error:"<<sqlite3_errmsg(db)<<std::endl;
        sqlite3_finalize(stmt);
        return false;
    }
    // Excute step.
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_ROW && rc != SQLITE_DONE)
    {
        // string errmsg(sqlite3_errmsg(db));
        std::cout<<" Read SQL error:"<<sqlite3_errmsg(db)<<std::endl;
        sqlite3_finalize(stmt);
        return false;
    }
    if (rc == SQLITE_DONE) {
        sqlite3_finalize(stmt);
        std::cout<<"customer not found"<<std::endl;
        return false;
    }

    // Assign dat to bsic struct.
    int columnname_size  = dbstruct.columnnames.size();    
    for (int i = 0; i < columnname_size; i++)
    {
        if (sqlite3_column_text(stmt, i)==NULL) // If get data == null,break the while loop.
        {
            return false;
        } 
        else
        {
            dbstruct.data[dbstruct.columnnames[i]] = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, i)));
        }
    }
    // Close sqlite3.
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return true;
}
/******************************************************************************************
 *
 *  @author  Otto
 *
 *  @date    2021/02/04
 *
 *  @fn      GtcsTcsDatabase::ReadDataBaseSequenceList(std::vector<GtcsTcsDatabaseSequenceInfo> &dblist,int jobid)
 *
 *  @brief   ( Constructivist )
 *
 *  @param   std::vector<GtcsTcsDatabaseSequenceInfo> &dblist
 * 
 *  @param   int jobid
 *
 *  @return  bool
 *
 *  @note    none
 *
 *******************************************************************************************/
bool GtcsTcsDatabase::ReadDataBaseSequenceList(std::vector<GtcsTcsDatabaseSequenceInfo> &dblist,int jobid)
{
    // Initial object 
    GtcsTcsDatabaseSequenceInfo sequencedata;
    dblist.clear();

    // Initial sqlcmd.
    std::string sqlcmd = "SELECT * from program ";
    sqlcmd += " where job_id=";
    sqlcmd += std::to_string(jobid);
    sqlcmd += ";";
    
    // Initial value.
    sqlite3 *db;
    sqlite3_stmt *stmt;
    int rc;
    // Open database.
    rc = sqlite3_open(dbPath.c_str(),&db);
    if (rc)
    {
        std::cout<<"Can't open database : "<< sqlite3_errmsg(db) <<std::endl;
        return false;
    }
    // Send SQL statement to db.
    rc = sqlite3_prepare_v2(db,sqlcmd.c_str(),-1,&stmt,NULL);
    if (rc != SQLITE_OK)
    {
        std::cout<<"Read SQL error:"<<sqlite3_errmsg(db)<<std::endl;
        sqlite3_finalize(stmt);
        return false;
    }

    // Excute sqlcmd Loop.  
    for(;;)
    {
        // Excute step.
        rc = sqlite3_step(stmt);    
        if (rc != SQLITE_ROW)
        {
            break;
        }
        else
        {
            // Assign dat to bsic struct.    
            int columnname_size  = sequencedata.columnnames.size();    
            for (int i = 0; i < columnname_size; i++)
            {
                if (sqlite3_column_text(stmt, i)==NULL) // If get data == null,break the while loop.
                {
                    return false;
                } 
                else
                {
                    sequencedata.data[sequencedata.columnnames[i]] = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, i)));
                }
            }
            dblist.push_back(sequencedata);
        }        
    }
    if (rc != SQLITE_DONE) 
    {
        sqlite3_finalize(stmt);
        std::cout<<"customer not found"<<std::endl;
        return false;
    }

    // Close sqlite3.
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return true;
}
/******************************************************************************************
 *
 *  @author  Otto
 *
 *  @date    2021/02/04
 *
 *  @fn      GtcsTcsDatabase::ReadDatabaseStepList(std::vector<GtcsTcsDatabaseStepInfo> &dblist,int jobid,int seqid)
 *
 *  @brief   ( Constructivist )
 *
 *  @param   vector<GtcsTcsDatabaseStepInfo> &dblist
 * 
 *  @param   int jobid
 * 
 *  @param   int seqid
 *
 *  @return  bool
 *
 *  @note    Get database step list.
 *
 *******************************************************************************************/
bool GtcsTcsDatabase::ReadDatabaseStepList(std::vector<GtcsTcsDatabaseStepInfo> &dblist,int jobid,int seqid)
{
    // Initial object 
    GtcsTcsDatabaseStepInfo stepdata;
    dblist.clear();

    // Initial sqlcmd.
    std::string sqlcmd = "SELECT * from step";
    sqlcmd += " where job_id=";
    sqlcmd += std::to_string(jobid);
    sqlcmd += " and ";
    sqlcmd += " seq_id=";
    sqlcmd += std::to_string(seqid);
    sqlcmd += ";";
    
    // Initial value.
    sqlite3 *db;
    sqlite3_stmt *stmt;
    int rc;
    // Open database.
    rc = sqlite3_open(dbPath.c_str(),&db);
    if (rc)
    {
        std::cout<<"Can't open database : "<< sqlite3_errmsg(db) <<std::endl;
        return false;
    }
    // Send SQL statement to db.
    rc = sqlite3_prepare_v2(db,sqlcmd.c_str(),-1,&stmt,NULL);
    if (rc != SQLITE_OK)
    {
        std::cout<<"Read SQL error:"<<sqlite3_errmsg(db)<<std::endl;
        sqlite3_finalize(stmt);
        return false;
    }

    // Excute sqlcmd Loop.  
    for(;;)
    {
        // Excute step.
        rc = sqlite3_step(stmt);    
        if (rc != SQLITE_ROW)
        {
            break;
        }
        else
        {
            // Assign dat to bsic struct.    
            int columnname_size  = stepdata.columnnames.size();    
            for (int i = 0; i < columnname_size; i++)
            {
                if (sqlite3_column_text(stmt, i)==NULL) // If get data == null,break the while loop.
                {
                    return false;
                } 
                else
                {
                    stepdata.data[stepdata.columnnames[i]] = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, i)));
                }
            }
            dblist.push_back(stepdata);
        }        
    }
    if (rc != SQLITE_DONE) 
    {
        sqlite3_finalize(stmt);
        std::cout<<"customer not found"<<std::endl;
        return false;
    }

    // Close sqlite3.
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return true;
}
#pragma endregion

#pragma region Gtcs screw status database.
/******************************************************************************************
 *
 *  @author  Otto
 *
 *  @date    2021/02/24
 *
 *  @fn      GtcsScrewStatusDatabase::GtcsScrewStatusDatabase()
 *
 *  @brief   ( Constructivist )
 *
 *  @param   std::string Path
 * 
 *  @return  none
 *
 *  @note    none.
 *
 *******************************************************************************************/
GtcsScrewStatusDatabase::GtcsScrewStatusDatabase(std::string Path)
{
    dbPath = Path;
}
/******************************************************************************************
 *
 *  @author  Otto
 *
 *  @date    2021/02/24
 *
 *  @fn      GtcsScrewStatusDatabase::GtcsScrewStatusDatabase()
 *
 *  @brief   ( Constructivist )
 *
 *  @param   none
 * 
 *  @return  none
 *
 *  @note    none.
 *
 *******************************************************************************************/
GtcsScrewStatusDatabase::~GtcsScrewStatusDatabase()
{}
/******************************************************************************************
 *
 *  @author  Otto
 *
 *  @date    2021/02/24
 *
 *  @fn      GtcsScrewStatusDatabase::CreatScrewStatusDatabaseTable()
 *
 *  @brief   ( Constructivist )
 *
 *  @param   none
 *
 *  @return  bool
 *
 *  @note    none
 *
 *******************************************************************************************/
bool GtcsScrewStatusDatabase::CreatScrewStatusDatabaseTable()
{
    // Initial sqlite object.
    sqlite3 *db;
    sqlite3_stmt *stmt;
    int rc;
    std::string sqlcmd = "";
    GtcsScrewStatusbaseInfo screwstatus;
    
    // create database.
    rc = sqlite3_open(dbPath.c_str(),&db);
    if (rc)
    {
        std::cout<<"Can't open GtcsScrewStatusDatabase : "<< sqlite3_errmsg(db) <<std::endl;
        return false;
    }
    // Creat database sql command.
    sqlcmd = GetDatabaseTableDefaultValue(screwstatus.dbtablename, screwstatus.columnnames,screwstatus.type);

    #if defined(_DEBUG_MODE_)
    // std::cout<< sqlcmd <<std::endl;
    #endif
    
    rc = sqlite3_prepare_v2(db, sqlcmd.c_str(),-1, &stmt, NULL);
    if (rc != SQLITE_OK)
    {
        std::cout<<"Read SQL error:"<<sqlite3_errmsg(db)<<std::endl;
        sqlite3_finalize(stmt);
        return false;
    }
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) 
    {
        sqlite3_finalize(stmt);
        std::cout<<"customer not found"<<std::endl;
        return false;
    }
    // Close sqlite3.
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return true;
}
/******************************************************************************************
 *
 *  @author  Otto
 *
 *  @date    2021/02/25
 *
 *  @fn      GtcsScrewStatusDatabase::InsertScrewStatusToDatabaseTable(GtcsScrewStatusbaseInfo &screwstatus)
 *
 *  @brief   ( Constructivist )
 *
 *  @param   GtcsScrewStatusbaseInfo &screwstatus
 *
 *  @return  bool
 *
 *  @note    none
 *
 *******************************************************************************************/
bool GtcsScrewStatusDatabase::InsertScrewStatusToDatabaseTable(GtcsScrewStatusbaseInfo &screwstatus)
{
// Initial sqlite object.
    sqlite3 *db;
    sqlite3_stmt *stmt;
    int rc;
    std::string sqlcmd = GetDatabaseInsertSqlCommand(screwstatus.dbtablename,
                                                    screwstatus.columnnames,
                                                    screwstatus.type,
                                                    screwstatus.data);

    // create database.
    rc = sqlite3_open(dbPath.c_str(),&db);
    if (rc)
    {
        std::cout<<"Can't open GtcsScrewStatusDatabase : "<< sqlite3_errmsg(db) <<std::endl;
        return false;
    }    

    #if defined(_DEBUG_MODE_)
    std::cout<< sqlcmd <<std::endl;
    #endif
    
    rc = sqlite3_prepare_v2(db, sqlcmd.c_str(),-1, &stmt, NULL);
    if (rc != SQLITE_OK)
    {
        std::cout<<"Read SQL error:"<<sqlite3_errmsg(db)<<std::endl;
        sqlite3_finalize(stmt);
        return false;
    }
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) 
    {
        sqlite3_finalize(stmt);
        std::cout<<"customer not found"<<std::endl;
        return false;
    }
    // Close sqlite3.
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return true;
}
#pragma endregion
