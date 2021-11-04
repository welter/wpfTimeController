#include "Sqlite3Helper.h"
using namespace std;

    Sqlite3Helper::Sqlite3Helper() {
    }
    Sqlite3Helper::~Sqlite3Helper() {
        sqlite3_close(db);
    }
    uint16_t Sqlite3Helper::openDatabase(const char* dbname) {
        return sqlite3_open(dbname, &db);
    }

    void Sqlite3Helper::closeDatabase() {
        sqlite3_close(db);
    }

    void Sqlite3Helper::new_table() {
        const char* sql;
        string name;
        string temp;
        cout << "please enter table's name" << endl;
        cin >> name;
        temp = "CREATE TABLE " + name +
            "(NAME          TEXT    NOT NULL," +
            "AGE            INT     NOT NULL);";
        sql = temp.c_str();
        rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
    }
    void Sqlite3Helper::add() {
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
        rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
    }
    void Sqlite3Helper::check() {
        const char* sql;
        string table_name, name, tmp;
        cout << "please input table's name" << endl;
        cin >> table_name;
        cout << "please input your name" << endl;
        cin >> name;
        tmp = "select AGE from " + table_name + " where NAME='" + name + "';";
        sql = tmp.c_str();
        rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
        cout << s << endl;
    }
    void Sqlite3Helper::loop() {
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
            switch (c) {
            case 1:new_table();
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