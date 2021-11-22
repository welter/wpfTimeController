#pragma once

#include <stdio.h>
#include <string>
#include <iostream>
#include <SQLiteCpp/SQLiteCpp.h> 
#include "common.h"
#include "RuleModel.h"
#include <vector>
namespace DB {
    using namespace std;
    using namespace SQLite;
    string s;
    int rc;

    class DBRuleService {
    private:
        SQLite::Database* db;
          bool newTable();
          SQLite::Statement* mQuery;
    public:DBRuleService();
          ~DBRuleService();
          bool getRule(DB::TimeControllerRule* rule, const char* ruleName);
          bool getRule(DB::TimeControllerRule* rule, int ruleID);
          int addRule(DB::TimeControllerRule* rule);
          bool setRule(const char* ruleName, DB::TimeControllerRule* rule);
          bool setRule(int ruleID, DB::TimeControllerRule* rule);
          bool deleteRule(int ruleID);
          bool clear();
    };
}