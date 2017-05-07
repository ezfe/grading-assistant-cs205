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
    DatabaseManager();
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

    bool is_active();
private:
    bool active = true;
    std::string path;
    sqlite3* current = nullptr;
};

#endif // DATABASEMANAGER_H
