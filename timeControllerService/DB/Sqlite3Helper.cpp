#include "ServiceDBHelper.h"
using namespace std;

    ServiceDBHelper::ServiceDBHelper() {}
    ServiceDBHelper::~ServiceDBHelper() {
        //if (db!=Null)  db.~Database();
    }
    Database* ServiceDBHelper::openDatabase(const char* dbname) {
        db = &Database(dbname, SQLite::OPEN_CREATE | SQLite::OPEN_READWRITE);     
        return db;

    }

    void ServiceDBHelper::closeDatabase() {
        //db->~Database();
    }

    void ServiceDBHelper::new_table(const char* tableName, const char* tableStruct[],int structSize) {
        const char* sql;
        string name;
        string temp="";
        string s;
        for (int i = 0; i++; i < structSize) {
            s = tableStruct[i];
            temp += s+",";
        }
        temp = temp.substr(0, temp.length() - 1);
        cout << "please enter table's name" << endl;
        cin >> name;
        try {
            SQLite::Database db(s, SQLite::OPEN_READWRITE, 0);

        }
        catch (std::exception& e)
        {
            //ConsolePrintf("exception: %s\n", e.what()); ConsoleScanf(ch, len);
            try {
                SQLite::Database db(s, SQLite::OPEN_CREATE | SQLite::OPEN_READWRITE, 0); 
            }
            catch (std::exception& e) {
                return;
            }
        }
        s = tableName;
        s= "CREATE TABLE " +s + "(" + temp.c_str() + "); ";
       db->exec("");
       c
        
        //temp = "CREATE TABLE " + name +
        //    "(NAME          TEXT    NOT NULL," +
        //    "AGE            INT     NOT NULL);";
        //sql = temp.c_str();
        //rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
    }
    void ServiceDBHelper::add() {
        const char* sql;
        string table_name, name, age, tmp;
        cout << "please input table's name" << endl;
        cin >> table_name;
        cout << "please input user's name" << endl;
        cin >> name;
        cout << "please input user's age" << endl;
        cin >> age;
        tmp = "insert into " + table_name + "(NAME, AGE)" +
            "values('" + name + "'," + age + ");";
        sql = tmp.c_str();
        rc = db->exec(sql);
    }
    void ServiceDBHelper::check() {
        const char* sql;
        string table_name, name, tmp;
        cout << "please input table's name" << endl;
        cin >> table_name;
        cout << "please input your name" << endl;
        cin >> name;
        tmp = "select AGE from " + table_name + " where NAME='" + name + "';";
        sql = tmp.c_str();
        rc = db->exec(sql);
        cout << s << endl;
    }
    void ServiceDBHelper::loop() {
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
            case 1:new_table("s",a, 1);
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

//gcc -shared sqlite3.c -o sqlite3.dll  win下链接
//gcc 1.cpp -lstdc++ sqlite3.c          win下编译  

//g++ 1.cpp -l sqlite3                  linux下链接编译