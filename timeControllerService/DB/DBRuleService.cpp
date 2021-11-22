#include "DBRuleService.h"
#include "RuleModel.h"
#include "DBSQL.h"
#include "common.h"
#include <array>
#include <vector>

namespace DB {
    using namespace std;
    const char* serviceDBPath = "test.db";

    DBRuleService::~DBRuleService() {
        if (db !=NULL)  db->~Database();
    }
   DBRuleService::DBRuleService() {
       try {
           db = &Database(serviceDBPath,SQLite::OPEN_READWRITE);
       }
        catch (std::exception& e)
        {
            //ConsolePrintf("exception: %s\n", e.what()); ConsoleScanf(ch, len);
            try {
                db = &Database(serviceDBPath, SQLite::OPEN_CREATE | SQLite::OPEN_READWRITE);
            }
            catch (std::exception& e) {
                return;
            }
        }
        if (!db->tableExists("RULE")) {
            newTable();
        }
    }

    bool DBRuleService::newTable() {
        const char* sql;
        s = SQL_CreateRuleTable; 
        return db->exec(s);
    }
    int DBRuleService::addRule(TimeControllerRule* rule) {
        const char* sql;
        string r;
        r = SQL_AddRule((*rule));
        sql = r.c_str();
        rc = db->exec(sql);
        return rc;
    }
    bool DBRuleService::deleteRule(int ruleID) {
        const char* sql;
        string r;
        r = SQL_DeleteRule(to_string(ruleID));
        sql = r.c_str();
        rc = db->exec(sql);
        return rc;
    }
    bool DBRuleService::clear() {
        const char* sql;
        string r;
        r = SQL_ClearRule;
        sql = r.c_str();
        rc = db->exec(sql);
        return rc;
    }
    bool DBRuleService::getRule(DB::TimeControllerRule* rule, const char* ruleName) {
        const char* sql;
        sql = SQL_GetRuleByName;
        mQuery=&SQLite::Statement(*db, sql);
        mQuery->bind("rulename",ruleName);
        //vector<TimeControllerRule*> a=vector<TimeControllerRule*>();       
        if (mQuery->executeStep())
        {
            //DB::TimeControllerRule* r=new DB::TimeControllerRule; 

            rule->SetId(mQuery->getColumn(0));
            rule->SetRuleName(mQuery->getColumn(1));
            rule->SetTaskName(mQuery->getColumn(2));
            rule->SetProgramTitle(mQuery->getColumn(3));
            rule->SetProgramDirectory(mQuery->getColumn(4));
            rule->SetRunPath(mQuery->getColumn(5));
            rule->SetRunningRule((DB::RunningRule) mQuery->getColumn(6).getInt());
            struct timeval t;
            t.tv_sec = mQuery->getColumn(7).getInt64();
            t.tv_usec = mQuery->getColumn(8).getInt64();
            rule->SetStartTime(t);
            t.tv_sec= mQuery->getColumn(9).getInt64();
            t.tv_usec = mQuery->getColumn(10).getInt64();
            rule->SetEndTime(t);
            rule->SetPerPeriodTime(mQuery->getColumn(11));
            rule->SetTimes(mQuery->getColumn(12));
            t.tv_sec = mQuery->getColumn(13).getInt64();
            t.tv_usec = mQuery->getColumn(14).getInt64();
            rule->SetTotalTime(t);
            rule->SetLimitRule((DB::LimitRule)mQuery->getColumn(15).getInt());
            //rule=&r;
        }
        return true;
        //rule = &a;
    }
    bool DBRuleService::getRule(DB::TimeControllerRule* rule,  int ruleID) {
        const char* sql;
        sql = SQL_GetRuleByID;
        mQuery = &SQLite::Statement(*db, sql);
        mQuery->bind("ruleid", ruleID);
        //vector<TimeControllerRule*> a=vector<TimeControllerRule*>();       
        if (mQuery->executeStep())
        {
            //DB::TimeControllerRule* r=new DB::TimeControllerRule; 

            rule->SetId(mQuery->getColumn(0));
            rule->SetRuleName(mQuery->getColumn(1));
            rule->SetTaskName(mQuery->getColumn(2));
            rule->SetProgramTitle(mQuery->getColumn(3));
            rule->SetProgramDirectory(mQuery->getColumn(4));
            rule->SetRunPath(mQuery->getColumn(5));
            rule->SetRunningRule((DB::RunningRule)mQuery->getColumn(6).getInt());
            struct timeval t;
            t.tv_sec = mQuery->getColumn(7).getInt64();
            t.tv_usec = mQuery->getColumn(8).getInt64();
            rule->SetStartTime(t);
            t.tv_sec = mQuery->getColumn(9).getInt64();
            t.tv_usec = mQuery->getColumn(10).getInt64();
            rule->SetEndTime(t);
            rule->SetPerPeriodTime(mQuery->getColumn(11));
            rule->SetTimes(mQuery->getColumn(12));
            t.tv_sec = mQuery->getColumn(13).getInt64();
            t.tv_usec = mQuery->getColumn(14).getInt64();
            rule->SetTotalTime(t);
            rule->SetLimitRule((DB::LimitRule)mQuery->getColumn(15).getInt());
            //rule=&r;
        }
        return true;
        //rule = &a;
    }

    bool DBRuleService::setRule(const char* ruleName, DB::TimeControllerRule* rule) {
        const char* sql;
        sql = SQL_SetRuleByName;
        mQuery = &SQLite::Statement(*db, sql);
        mQuery->bind("taskname", rule->GetTaskName());
        mQuery->bind("programtile", rule->GetProgramTitle());
        mQuery->bind("programdirectory", rule->GetProgramDirectory());
        mQuery->bind("runpath", rule->GetRunPath());
        mQuery->bind("runningrule", rule->GetRunningRule());
        struct timeval t;
        t = rule->GetStartTime();
        mQuery->bind("starttimesec", t.tv_sec);
        mQuery->bind("starttimeusec", t.tv_usec);
        t = rule->GetEndTime();
        mQuery->bind("endtimesec", t.tv_sec);
        mQuery->bind("endtimeusec", t.tv_usec);
        mQuery->bind("perperiodtime", rule->GetPerPeriodTime());
        mQuery->bind("times", rule->GetTimes());
        t = rule->GetTotalTime();
        mQuery->bind("totaltimesec", t.tv_sec);
        mQuery->bind("totaltimeusec", t.tv_usec);
        mQuery->bind("limitrule", rule->GetLimitRule());

        mQuery->bind("rulename", ruleName);
        mQuery->exec();
        return true;
    }
    bool DBRuleService::setRule(int ruleID, DB::TimeControllerRule* rule) {
        const char* sql;
        sql = SQL_SetRuleByID;
        mQuery = &SQLite::Statement(*db, sql);
        mQuery->bind("taskname", rule->GetTaskName());
        mQuery->bind("programtile", rule->GetProgramTitle());
        mQuery->bind("programdirectory", rule->GetProgramDirectory());
        mQuery->bind("runpath", rule->GetRunPath());
        mQuery->bind("runningrule", rule->GetRunningRule());
        struct timeval t;
        t = rule->GetStartTime();
        mQuery->bind("starttimesec", t.tv_sec);
        mQuery->bind("starttimeusec", t.tv_usec);
        t = rule->GetEndTime();
        mQuery->bind("endtimesec", t.tv_sec);
        mQuery->bind("endtimeusec", t.tv_usec);
        mQuery->bind("perperiodtime", rule->GetPerPeriodTime());
        mQuery->bind("times", rule->GetTimes());
        t = rule->GetTotalTime();
        mQuery->bind("totaltimesec", t.tv_sec);
        mQuery->bind("totaltimeusec", t.tv_usec);
        mQuery->bind("limitrule", rule->GetLimitRule());
        mQuery->bind("rulename", rule->GetRuleName());

        mQuery->bind("ruleid", ruleID);
        mQuery->exec();
        return true;
    }
    
}
