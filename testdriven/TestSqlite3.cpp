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
#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h> 

static int callback(void *data, int argc, char **argv, char **azColName){
   int i;
   fprintf(stderr, "%s: ", (const char*)data);
   for(i=0; i<argc; i++){
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   printf("\n");
   return 0;
}

int main(int argc, char* argv[])
{
   sqlite3 *db;
   char *zErrMsg = 0;
   int rc;
   // char *sql;
   const char* data = "Callback function called";

   /* Open database */
   rc = sqlite3_open("test.db", &db);
   if( rc ){
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
      exit(0);
   }else{
      fprintf(stderr, "Opened database successfully\n");
   }

   /* Create merged SQL statement */
   char sql[] = "DELETE from COMPANY where ID=2; " \
         "SELECT * from COMPANY";

   /* Execute SQL statement */
   rc = sqlite3_exec(db, sql, callback, (void*)data, &zErrMsg);
   if( rc != SQLITE_OK ){
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
   }else{
      fprintf(stdout, "Operation done successfully\n");
   }
   sqlite3_close(db);
   return 0;
}
#pragma endregion
