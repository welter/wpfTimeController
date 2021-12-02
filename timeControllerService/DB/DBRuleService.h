#pragma once

#include <stdio.h>
#include <string>
#include <iostream>
#include <SQLiteCpp/SQLiteCpp.h>
#include "common.h"
#include "RuleModel.h"
#include <vector>
namespace DB {


    class DBRuleService {
    private:
          SQLite::Database openTable();
    public:DBRuleService() =default;
          ~DBRuleService() =default;
          bool getRule(DB::TimeControllerRule* rule, const char* ruleName);
          bool getRule(DB::TimeControllerRule* rule, int ruleID);
          int addRule(DB::TimeControllerRule* rule);
          bool setRule(const char* ruleName, DB::TimeControllerRule* rule);
          bool setRule(int ruleID, DB::TimeControllerRule* rule);
          bool deleteRule(int ruleID);
          bool deleteRule(const char* ruleName);
          bool clear();
    };
}