#ifndef DATABASETABLE_H
#define DATABASETABLE_H

#include <string>
#include <vector>
#include <iostream>

#include <sqlite3.h>

#include "databasemanager.h"

class DatabaseTable {
public:
    DatabaseTable(DatabaseManager* manager, std::string name, std::string schema);

    bool drop();
    bool create();
    bool recreate();

    std::string get_name();

    std::string prepare_query(std::string schema, std::string where);
    std::string prepare_query(std::string schema);
    std::string prepare_select_all(std::string where);
    std::string prepare_select_all();

    std::string prepare_insert(std::string columns, std::string values);
    bool insert(std::string columns, std::string values);

    sqlite3_stmt* prepare_statement(std::string query);
    static int get_int(sqlite3_stmt* statement, int column);
    static std::string get_string(sqlite3_stmt* statement, int column);
    static void finalize_statement(sqlite3_stmt* statement);

    int single_exec(std::string query);

    bool delete_row_wid(std::string id);
    bool delete_row(std::string where);

    static std::string escape_string(std::string string);

    std::string get_sort();
    bool is_sorted();
    void set_sort(std::string sort);
    void unset_sort();
private:
    std::string name;
    std::string create_sql;

    bool sort = false;
    std::string sort_str = "";

    DatabaseManager* database;

    DatabaseTable();
};

#endif // DATABASETABLE_H
