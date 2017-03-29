#ifndef DATABASETABLE_H
#define DATABASETABLE_H

#include <string>
#include <sqlite3.h>
#include <vector>

#include "databasemanager.h"

class DatabaseTable {
public:
    DatabaseTable(DatabaseManager* manager, std::string name, std::vector<std::string> schema);
    DatabaseTable(DatabaseManager* manager, std::string name, std::string create_sql);

    bool drop();
    bool create();
private:
    std::string name;
    std::string create_sql;

    DatabaseManager* database;

    DatabaseTable();
};

#endif // DATABASETABLE_H
