#pragma once
#include <stdio.h>
#include <string>
#include <iostream>
#include <sqlite3.h> 
using namespace std;
string s;
int callback(void* para, int nCount, char** pValue, char** pName) {
     s = "";
    for (uint16_t i = 0; i < nCount; i++) {
        s += pValue[i];
    }
    return 0;
}

class Sqlite3Helper {
private:
    sqlite3* db;
    char* zErrMsg = 0;
    uint16_t rc;

public:Sqlite3Helper();
      ~Sqlite3Helper();
      uint16_t openDatabase(const char* dbname);
      void closeDatabase();
      void new_table();
      void add();
      void check();
      void loop();
};