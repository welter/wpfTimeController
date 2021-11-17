#include "DBRuleService.h"
#include "RuleModel.h"
#include "DBSQL.h"
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
        string table_name, name, age, tmp;
        tmp = "insert into " + table_name + "(NAME, AGE)" +
            "values('" + name + "'," + age + ");";
        sql = tmp.c_str();
        rc = db->exec(sql);
        return rc;
    }
    bool DBRuleService::deleteRule(int ruleID) {
        const char* sql;
        sql = SQL_DeleteRule;
        rc = db->exec(sql);
        return rc;
    }
    void DBRuleService::loop() {
        int c;
        while (1) {
            cout << "-----------------------------\n" <<
                "name & age\n" <<
                "1 to add new table\n" <<
                "2 to insert information\n" <<
                "3 to check information\n" <<
                "4 to quit\n" <<
                "-----------------------------\n";
            cin >> c;
            const char* a[5];
            switch (c) {
            case 1:new_table("s", a, 1);
                break;
            case 2:add();
                break;
            case 3:check();
                break;
            case 4:return;
                break;
            default:cout << "\nplease enter again!" << endl;
            }
        }
    }
}
//gcc -shared sqlite3.c -o sqlite3.dll  win下链接
//gcc 1.cpp -lstdc++ sqlite3.c          win下编译  

//g++ 1.cpp -l sqlite3                  linux下链接编译