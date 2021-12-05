cmake_minimum_required (VERSION 3.20)
#find_package(unofficial-sqlite3 CONFIG REQUIRED)

find_package(SQLiteCpp CONFIG REQUIRED)
ADD_LIBRARY ( DB STATIC "DBRuleService.cpp" "DBRuleService.h" "RuleModel.h" "common.h" "RuleModel.cpp" "DBSQL.h")
#target_link_libraries(helper PRIVATE unofficial::sqlite3::sqlite3)
target_link_libraries(DB PRIVATE SQLiteCpp)
 
add_executable (dbtest WIN32 "dbtest.cpp" "dbtest.h")
target_include_directories(dbtest PUBLIC C:/vcpkg/installed/x64-windows/include)
TARGET_LINK_LIBRARIES(dbtest PRIVATE "DB")
TARGET_LINK_LIBRARIES(dbtest PRIVATE "Timer")