#ifndef DATABASETABLE_H
#define DATABASETABLE_H

#include <string>
#include <sqlite3.h>
#include <vector>

#include "databasemanager.h"

class DatabaseTable {
public:
    DatabaseTable(DatabaseManager* manager, std::string name, std::string schema);

    bool drop();
    bool create();

    std::string get_name();

    std::string prepare_query(std::string schema, std::string where);
    std::string prepare_query(std::string schema);
    std::string prepare_select_all(std::string where);
    std::string prepare_select_all();

    std::string prepare_insert(std::string columns, std::string values);
    bool insert(std::string columns, std::string values);

    sqlite3_stmt* prepare_statement(std::string query);
    int get_int(sqlite3_stmt* statement, int column);
    std::string get_string(sqlite3_stmt* statement, int column);
    void finalize_statement(sqlite3_stmt* statement);

    int single_exec(std::string query);

    static std::string escape_string(std::string string);
private:
    std::string name;
    std::string create_sql;

    DatabaseManager* database;

    DatabaseTable();
};

#endif // DATABASETABLE_H
