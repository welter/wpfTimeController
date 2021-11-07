#pragma once
#include <stdio.h>
#include <string>
#include <iostream>
#include <SQLiteCpp/SQLiteCpp.h> 
using namespace std;
using namespace SQLite;
string s;
int callback(void* para, int nCount, char** pValue, char** pName) {
     s = "";
    for (uint16_t i = 0; i < nCount; i++) {
        s += pValue[i];
    }
    return 0;
}struct cstruct {
    const char* name;
    int type;
    int clong;
    int dlong;
} ;

class Sqlite3Helper {
private:
    SQLite::Database* db;
    char* zErrMsg = 0;
    uint16_t rc;

public:Sqlite3Helper();
      ~Sqlite3Helper();
      Database* openDatabase(const char* dbname);
      void closeDatabase();
      void new_table(const char* tableName,const char* tableStruct[],int structSize);
      void add();
      void check();
      void loop();
};