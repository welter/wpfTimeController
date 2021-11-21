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
        if (database !=NULL)  database->~Database();
    }
   DBRuleService::DBRuleService() {
       try {
           database = &Database(serviceDBPath,SQLite::OPEN_READWRITE);
       }
        catch (std::exception& e)
        {
            //ConsolePrintf("exception: %s\n", e.what()); ConsoleScanf(ch, len);
            try {
                database = &Database(serviceDBPath, SQLite::OPEN_CREATE | SQLite::OPEN_READWRITE);
            }
            catch (std::exception& e) {
                return;
            }
        }
        if (!database->tableExists("RULE")) {
            newTable();
        }
    }

    bool DBRuleService::newTable() {
        const char* sql;
        s = SQL_CreateRuleTable; 
        return database->exec(s);
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
    bool DBRuleService::getRules(DB::TimeControllerRule** *rule, const char* ruleName) {
        const char* sql;
        sql = "asdf";
        mQuery=&SQLite::Statement(*database, sql);
        mQuery->bind("");
        vector<TimeControllerRule*> a;
        DB::TimeControllerRule* p = new DB::TimeControllerRule [];            
        a.push_back(r);
        while (mQuery->executeStep())
        {
            DB::TimeControllerRule* r=new DB::TimeControllerRule; 
            r->SetId(mQuery->getColumn(0));
            r->SetRuleName(mQuery->getColumn(1));
            r->SetTaskName(mQuery->getColumn(2));
            r->SetProgramTitle(mQuery->getColumn(3));
            r->SetProgramDirectory(mQuery->getColumn(4));
            r->SetRunPath(mQuery->getColumn(5));
            r->SetRunningRule((DB::RunningRule) mQuery->getColumn(6).getInt());
            struct timeval t;
            t.tv_sec = mQuery->getColumn(7).getInt64();
            t.tv_usec = mQuery->getColumn(8).getInt64();
            r->SetStartTime(t);
            t.tv_sec= mQuery->getColumn(9).getInt64();
            t.tv_usec = mQuery->getColumn(10).getInt64();
            r->SetEndTime(t);
            r->SetPerPeriodTime(mQuery->getColumn(11));
            t.tv_sec = mQuery->getColumn(12).getInt64();
            t.tv_usec = mQuery->getColumn(13).getInt64();
            r->SetTotalTime(t);
            r->SetLimitRule((DB::LimitRule)mQuery->getColumn(14).getInt());
        }
        DB::TimeControllerRule* p;
        p = *(rule[2]);
        

        
        
    }
}
