#include "databasetable.h"

DatabaseTable::DatabaseTable() {

}

DatabaseTable::DatabaseTable(DatabaseManager* manager, std::string name) {
    this->database = manager;
    this->name = name;

    this->create();
}

bool DatabaseTable::drop() {
    int sqlCode = SQLITE_ERROR;

    std::string sqlCommand = "DROP TABLE IF EXISTS " + this->name + ";";
    sqlCode = sqlite3_exec(this->database->db(), sqlCommand.c_str(), 0, 0, nullptr);

    if (sqlCode == SQLITE_OK) {
        return true;
    } else {
        this->database->dberror("Unable to drop database (#1)");
        return false;
    }
}

bool DatabaseTable::create() {
    int sqlCode = SQLITE_ERROR;

    sqlCode = sqlite3_exec(this->database->db(), this->create_sql().c_str(), 0, 0, nullptr);

    if (sqlCode == SQLITE_OK) {
        return true;
    } else {
        this->database->dberror("Unable to drop database (#1)");
        return false;
    }
}
