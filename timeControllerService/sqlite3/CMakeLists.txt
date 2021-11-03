find_package(unofficial-sqlite3 CONFIG REQUIRED)
add_executable ( helper "helper.cpp")
target_link_libraries(helper PRIVATE unofficial::sqlite3::sqlite3)
