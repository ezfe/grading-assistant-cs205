#ifndef DATABASETABLE_H
#define DATABASETABLE_H

#include <string>
#include <sqlite3.h>

#include "databasemanager.h"

class DatabaseTable {
public:
    DatabaseTable(DatabaseManager* manager, std::string name);

    virtual std::string create_sql() = 0;

    bool drop();
    bool create();
protected:
    std::string name;
    DatabaseManager* database;

private:
    DatabaseTable();
};

#endif // DATABASETABLE_H
