#pragma once

#include <stdio.h>
#include <string>
#include <iostream>
#include <SQLiteCpp/SQLiteCpp.h> 
#include "common.h"
#include "RuleModel.h"
namespace DB {
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
    };





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

    class DBRuleService {
    private:
        SQLite::Database* db;
        char* zErrMsg = 0;
        uint16_t rc;
          bool newTable();
    public:DBRuleService();
          ~DBRuleService();
          Database* database;
          bool getRule(const char* ruleName);
          bool getRule(int ruleID);
          int addRule(DB::TimeControllerRule rule);
          bool setRule(const char* ruleName, DB::TimeControllerRule rule);
          bool setRule(int ruleID, DB::TimeControllerRule rule);
          bool deleteRule(int ruleID);
          bool clear();
    };
}