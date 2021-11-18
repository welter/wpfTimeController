#include "DBRuleService.h"
#include "RuleModel.h"
#include "DBSQL.h"
#include "common.h"
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
    int DBRuleService::addRule(TimeControllerRule rule) {
        const char* sql;
        string r;
        r = SQL_AddRule(rule);
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
}
