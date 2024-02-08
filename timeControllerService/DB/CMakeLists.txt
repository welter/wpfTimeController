cmake_minimum_required (VERSION 3.20)
#find_package(unofficial-sqlite3 CONFIG REQUIRED)

find_package(SQLiteCpp CONFIG REQUIRED)
ADD_LIBRARY ( DB STATIC "RuleService.cpp" "RuleService.h" "RuleModel.h" "common.h" "RuleModel.cpp" "DBSQL.h")
#target_link_libraries(helper PRIVATE unofficial::sqlite3::sqlite3)
target_link_libraries(DB PRIVATE SQLiteCpp)





INSTALL(FILES log.s3db rule.db DESTINATION ${CMAKE_CURRENT_BINARY_DIR})