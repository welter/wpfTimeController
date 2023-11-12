#pragma once

#include <stdio.h>
#include <string>
#include <iostream>
#include <SQLiteCpp/SQLiteCpp.h>
#include "common.h"
#include "RuleModel.h"
#include <vector>
namespace DB {


    class RuleService {
    private:
          
    public:
          bool openTable();
          bool closeTable();
          RuleService() =default;
          ~RuleService() =default;
          bool getRule(DB::TimeControllerRule* rule, const char* ruleName);
          bool getRule(DB::TimeControllerRule* rule, int ruleID);
          int addRule(DB::TimeControllerRule* rule);
          bool setRule(const char* ruleName, DB::TimeControllerRule* rule);
          bool setRule(int ruleID, DB::TimeControllerRule* rule);
          bool deleteRule(int ruleID);
          bool deleteRule(const char* ruleName);
          bool clear();
          int getRuleCount();
          bool getAllRule(DB::TimeControllerRule*** rules,int& ruleCount);
    };
}