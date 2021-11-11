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

struct timeval
{
    long tv_sec; /*��*/
    long tv_usec; /*΢��*/
};

enum RunningRule {};
enum LimitRule {};

struct tcsRule {
    long Id;
    string TaskName;
    string ProgramTitle;
    string ProgramDirectory;
    string RunPath;
    RunningRule Rule;
    timeval StartTime;
    timeval EndTime;
    timeval PerPeriodTime;
    int Times;
    timeval TotalTime;
    LimitRule Limit;

};

class ServiceDBHelper {
private:
    SQLite::Database* db;
    char* zErrMsg = 0;
    uint16_t rc;

public:ServiceDBHelper();
      ~ServiceDBHelper();
      Database* openDatabase();
      void closeDatabase();
      void getRule(const char* ruleName);
      void getRule(int ruleID);
      void addRule(tcsRule rule);
      void setRule(const char* ruleName, tcsRule rule);
      void setRule(int ruleID, tcsRule rule);

      void new_table(const char* tableName,const char* tableStruct[],int structSize);
      void add();
      void check();
      void loop();
};