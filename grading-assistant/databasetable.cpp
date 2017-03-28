#include "databasetable.h"

DatabaseTable::DatabaseTable() {

}

DatabaseTable::DatabaseTable(DatabaseManager* manager, std::string name) {
    this->database = manager;
    this->name = name;
}

void DatabaseTable::drop() {
    int sqlCode = SQLITE_ERROR;

    std::string sqlCommand = "DROP TABLE IF EXISTS " + this->name + ";";
    sqlite3_stmt* sqlStatement = nullptr;
    sqlCode = sqlite3_prepare_v2(this->database->db(), sqlCommand.c_str(), -1, &sqlStatement, nullptr);
    if (sqlCode == SQLITE_OK && sqlStatement != nullptr) {
        if (sqlite3_step(sqlStatement) != SQLITE_DONE) {
            this->database->dberror("Unable to drop database");
        }
    } else {
        this->database->dberror("Unable to drop database");
    }
    sqlite3_finalize(sqlStatement);
}
