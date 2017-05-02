#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <string>
#include <iostream>

#include <sqlite3.h>

/*!
 * \brief A container for an sqlite3 database
 */
class DatabaseManager {
public:
    DatabaseManager(std::string path);

    ~DatabaseManager();

    std::string get_path();
    void set_path(std::string path);

    void open();
    void close();

    bool is_open();

    sqlite3* db();

    void dberror();
    void dberror(std::string message);
private:
    DatabaseManager();

    std::string path;
    sqlite3* current;
};

#endif // DATABASEMANAGER_H
