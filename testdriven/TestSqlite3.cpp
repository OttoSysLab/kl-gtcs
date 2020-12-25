/*
// cmd:$g++ test.c -l sqlite3
*/
#pragma region Create sqlite database.
// #include <stdio.h>
// #include <sqlite3.h>

// int main(int argc, char* argv[])
// {
//    sqlite3 *db;
//    char *zErrMsg = 0;
//    int rc;
//    rc = sqlite3_open("test.db", &db);

//    if( rc ){
//       fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
//       return 0;
//    }else{
//       fprintf(stderr, "Opened database successfully\n");
//    }
//    sqlite3_close(db);
//    return 0;
// }
#pragma endregion

#pragma region Create sqlite database table.
// #include <stdio.h>
// #include <stdlib.h>
// #include <sqlite3.h> 

// static int callback(void *NotUsed, int argc, char **argv, char **azColName){
//    int i;
//    for(i=0; i<argc; i++){
//       printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
//    }
//    printf("\n");
//    return 0;
// }

// int main(int argc, char* argv[])
// {
//    sqlite3 *db;
//    char *zErrMsg = 0;
//    int  rc;
//    // char *sql;

//    /* Open database */
//    rc = sqlite3_open("test.db", &db);
//    if( rc ){
//       fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
//       exit(0);
//    }else{
//       fprintf(stdout, "Opened database successfully\n");
//    }

//    /* Create SQL statement */
//    char sql[] = "CREATE TABLE COMPANY("  \
//          "ID INT PRIMARY KEY     NOT NULL," \
//          "NAME           TEXT    NOT NULL," \
//          "AGE            INT     NOT NULL," \
//          "ADDRESS        CHAR(50)," \
//          "SALARY         REAL );";

//    /* Execute SQL statement */
//    rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
//    if( rc != SQLITE_OK ){
//    fprintf(stderr, "SQL error: %s\n", zErrMsg);
//       sqlite3_free(zErrMsg);
//    }else{
//       fprintf(stdout, "Table created successfully\n");
//    }
//    sqlite3_close(db);
//    return 0;
// }
#pragma endregion

#pragma region Insert data to sqlite database. 
// #include <stdio.h>
// #include <stdlib.h>
// #include <sqlite3.h>

// static int callback(void *NotUsed, int argc, char **argv, char **azColName){
//    int i;
//    for(i=0; i<argc; i++){
//       printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
//    }
//    printf("\n");
//    return 0;
// }

// int main(int argc, char* argv[])
// {
//    sqlite3 *db;
//    char *zErrMsg = 0;
//    int rc;
//    // char *sql;

//    /* Open database */
//    rc = sqlite3_open("test.db", &db);
//    if( rc ){
//       fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
//       exit(0);
//    }else{
//       fprintf(stderr, "Opened database successfully\n");
//    }

//    /* Create SQL statement */
//    char sql[] = "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY) "  \
//          "VALUES (1, 'Paul', 32, 'California', 20000.00 ); " \
//          "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY) "  \
//          "VALUES (2, 'Allen', 25, 'Texas', 15000.00 ); "     \
//          "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY)" \
//          "VALUES (3, 'Teddy', 23, 'Norway', 20000.00 );" \
//          "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY)" \
//          "VALUES (4, 'Mark', 25, 'Rich-Mond ', 65000.00 );";

//    /* Execute SQL statement */
//    rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
//    if( rc != SQLITE_OK ){
//       fprintf(stderr, "SQL error: %s\n", zErrMsg);
//       sqlite3_free(zErrMsg);
//    }else{
//       fprintf(stdout, "Records created successfully\n");
//    }
//    sqlite3_close(db);
//    return 0;
// }
#pragma endregion 

#pragma region Select data for sqlite database.
// #include <stdio.h>
// #include <stdlib.h>
// #include <sqlite3.h>

// static int callback(void *data, int argc, char **argv, char **azColName){
//    int i;
//    fprintf(stderr, "%s: ", (const char*)data);
//    for(i=0; i<argc; i++){
//       printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
//    }
//    printf("\n");
//    return 0;
// }

// int main(int argc, char* argv[])
// {
//    sqlite3 *db;
//    char *zErrMsg = 0;
//    int rc;
//    // char *sql;
//    const char* data = "Callback function called";

//    /* Open database */
//    rc = sqlite3_open("test.db", &db);
//    if( rc ){
//       fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
//       exit(0);
//    }else{
//       fprintf(stderr, "Opened database successfully\n");
//    }

//    /* Create SQL statement */
//    char sql[] = "SELECT * from COMPANY";

//    /* Execute SQL statement */
//    rc = sqlite3_exec(db, sql, callback, (void*)data, &zErrMsg);
//    if( rc != SQLITE_OK ){
//       fprintf(stderr, "SQL error: %s\n", zErrMsg);
//       sqlite3_free(zErrMsg);
//    }else{
//       fprintf(stdout, "Operation done successfully\n");
//    }
//    sqlite3_close(db);
//    return 0;
// }
#pragma endregion

#pragma region Update sqlite database.
// #include <stdio.h>
// #include <stdlib.h>
// #include <sqlite3.h> 

// static int callback(void *data, int argc, char **argv, char **azColName){
//    int i;
//    fprintf(stderr, "%s: ", (const char*)data);
//    for(i=0; i<argc; i++){
//       printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
//    }
//    printf("\n");
//    return 0;
// }

// int main(int argc, char* argv[])
// {
//    sqlite3 *db;
//    char *zErrMsg = 0;
//    int rc;
//    // char *sql;
//    const char* data = "Callback function called";

//    /* Open database */
//    rc = sqlite3_open("test.db", &db);
//    if( rc ){
//       fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
//       exit(0);
//    }else{
//       fprintf(stderr, "Opened database successfully\n");
//    }

//    /* Create merged SQL statement */
//    char sql[] = "UPDATE COMPANY set SALARY = 25000.00 where ID=1; " \
//          "SELECT * from COMPANY";

//    /* Execute SQL statement */
//    rc = sqlite3_exec(db, sql, callback, (void*)data, &zErrMsg);
//    if( rc != SQLITE_OK ){
//       fprintf(stderr, "SQL error: %s\n", zErrMsg);
//       sqlite3_free(zErrMsg);
//    }else{
//       fprintf(stdout, "Operation done successfully\n");
//    }
//    sqlite3_close(db);
//    return 0;
// }
#pragma endregion


#pragma region Delete sqlite data.
// #include <stdio.h>
// #include <stdlib.h>
// #include <sqlite3.h> 

// static int callback(void *data, int argc, char **argv, char **azColName){
//    int i;
//    fprintf(stderr, "%s: ", (const char*)data);
//    for(i=0; i<argc; i++){
//       printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
//    }
//    printf("\n");
//    return 0;
// }

// int main(int argc, char* argv[])
// {
//    sqlite3 *db;
//    char *zErrMsg = 0;
//    int rc;
//    // char *sql;
//    const char* data = "Callback function called";

//    /* Open database */
//    rc = sqlite3_open("test.db", &db);
//    if( rc ){
//       fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
//       exit(0);
//    }else{
//       fprintf(stderr, "Opened database successfully\n");
//    }

//    /* Create merged SQL statement */
//    char sql[] = "DELETE from COMPANY where ID=2; " \
//          "SELECT * from COMPANY";

//    /* Execute SQL statement */
//    rc = sqlite3_exec(db, sql, callback, (void*)data, &zErrMsg);
//    if( rc != SQLITE_OK ){
//       fprintf(stderr, "SQL error: %s\n", zErrMsg);
//       sqlite3_free(zErrMsg);
//    }else{
//       fprintf(stdout, "Operation done successfully\n");
//    }
//    sqlite3_close(db);
//    return 0;
// }
#pragma endregion

#pragma region 
// #include "sqlite3.h" 
// #include <cstdio>
// #include <cstring>
 
// #include <assert.h>  
// #pragma comment(lib, "sqlite3.lib")  
 
// typedef struct per
// {
//     char *name;
//     int age;
//     char *sex;
// } per;
 
// per a[] = {
//     "David",22,"man",
//     "Eve",28,"man",
//     "Frand",21,"woman"
// };
 
 
 
// // 这个函数可以用来打印出每行的信息  
// static int callback(void *NotUsed, int argc, char **argv, char **azColName){  
//     int i;  
//     for(i=0; i<argc; i++){  
//         printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");  
//     }  
//     printf("\n");  
//     return 0;  
// }  
 
// int main()  
// {  
//     sqlite3 *pdb;  
//     char *zErrMsg;  
//     sqlite3_stmt *pstmt;  
//     const char *pzTail;  
//     const unsigned char *pTmp;
//     int age;
//     int nCol;
 
//     // 打开数据库连接   
//     int nRet = sqlite3_open("people.db",&pdb);  
//     assert(SQLITE_OK == nRet);  
 
//     // 创建表  
//     const char *sql = "CREATE TABLE IF NOT EXISTS person(name VARCHAR(128),"  
//         "age INTEGER,"  
//         "sex VARCHAR(7)"  
//         ");";  
//     nRet = sqlite3_exec(pdb,sql,NULL,NULL,&zErrMsg);  
//     if (nRet != SQLITE_OK)  
//     {  
//         printf("%s\n",zErrMsg);  
//         sqlite3_close(pdb);  
//         return 1;  
//     }
 
//     sql = "DELETE  FROM person;";
//     nRet = sqlite3_exec(pdb,sql,NULL,NULL,&zErrMsg);  
//     if (nRet != SQLITE_OK)  
//     {  
//         printf("%s\n",zErrMsg);  
//         sqlite3_close(pdb);  
//         return 1;  
//     }
 
//     // 使用sqlite3_exec() 插入数据  
//     sql = "INSERT INTO person(name,age,sex) VALUES(\"Alice\",15,\"woman\");";  
//     nRet = sqlite3_exec(pdb,sql,NULL,NULL,&zErrMsg);  
//     assert(SQLITE_OK == nRet);  // 为了简单，这里不打印zErrMsg的值  
 
//     sql = "INSERT INTO person(name,age,sex) VALUES(\"Bob\",18,\"man\");";  
//     nRet = sqlite3_exec(pdb,sql,NULL,NULL,&zErrMsg);  
//     assert(SQLITE_OK == nRet);  // 为了简单，这里不打印zErrMsg的值  
 
//     sql = "INSERT INTO person(name,age,sex) VALUES(\"Charli\",11,\"man\");";  
//     nRet = sqlite3_exec(pdb,sql,NULL,NULL,&zErrMsg);  
//     assert(SQLITE_OK == nRet);  // 为了简单，这里不打印zErrMsg的值  
 
//     // 使用sqlite3_exec() 查询数据  
//     printf("=====query by sqlite3_exec()=====\n");
//     sql = "SELECT name,age,sex FROM person;";  
//     nRet = sqlite3_exec(pdb,sql,callback,NULL,&zErrMsg);  
//     assert(SQLITE_OK == nRet);  
 
//     // 使用sqlite3_prepare_v2(), sqlite3_bind_...() 插入数据
//     sql = "INSERT INTO person(name,age,sex) VALUES(?,?,?);";
//     nRet = sqlite3_prepare_v2(pdb,sql,strlen(sql),&pstmt,&pzTail);
//     assert(SQLITE_OK == nRet);  
//     int i;
//     for (i=0;i<sizeof(a)/sizeof(per);i++)
//     {
//         nCol = 1;
//         sqlite3_bind_text(pstmt,nCol++,a[i].name,strlen(a[i].name),NULL);
//         sqlite3_bind_int(pstmt,nCol++,a[i].age);
//         sqlite3_bind_text(pstmt,nCol++,a[i].sex,strlen(a[i].sex),NULL);
 
//         sqlite3_step(pstmt);
//         sqlite3_reset(pstmt);
//     }
 
//     sqlite3_finalize(pstmt);
 
//     // 使用sqlite3_prepare_v2(), sqlite3_column_...() 查询数据
 
//     printf("====== query by sqlite3_prepare_v2()======\n");
//     sql = "SELECT name,age,sex FROM person;";
//     nRet = sqlite3_prepare_v2(pdb,sql,strlen(sql),&pstmt,&pzTail);
//     assert(SQLITE_OK == nRet);  
    
//     while(sqlite3_step(pstmt) == SQLITE_ROW)
//     {
//         nCol = 0;
//         pTmp = sqlite3_column_text(pstmt,nCol++);
//         printf("%s|",pTmp);
 
//         age = sqlite3_column_int(pstmt,nCol++);
//         printf("%d|",age);
 
//         pTmp = sqlite3_column_text(pstmt,nCol++);
//         printf("%s\n",pTmp);
//     }
//     sqlite3_finalize(pstmt);
 
//     sqlite3_close(pdb);  
//     return 0; 
// }
#pragma endregion

#pragma region 
// map::begin/end
#include <iostream>
#include <map>

int main ()
{
  std::map<char,int> mymap;

  mymap['b'] = 100;
  mymap['a'] = 200;
  mymap['c'] = 300;

  // show content:
  for (std::map<char,int>::iterator it=mymap.begin(); it!=mymap.end(); ++it)
    std::cout << it->first << " => " << it->second << '\n';

  return 0;
}
#pragma endregion
