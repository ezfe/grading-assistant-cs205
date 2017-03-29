#include "databasetable.h"

DatabaseTable::DatabaseTable() {
    std::cerr << "Must provide parameters" << std::endl;
}

DatabaseTable::DatabaseTable(DatabaseManager* manager, std::string name, std::vector<std::string> schema) {
    this->name = name;

    this->create_sql = "CREATE TABLE IF NOT EXISTS " + this->name + " (";
    for(int i = 0; i < schema.size(); i++) {
        std::string element = schema[i];
        this->create_sql += element;
        if (i < schema.size() - 1) {
            this->create_sql += ", ";
        }
    }
    this->create_sql += ");";

    this->database = manager;

    this->create();
}

DatabaseTable::DatabaseTable(DatabaseManager *manager, std::string name, std::string create_sql) {
    this->name = name;
    this->create_sql = create_sql;
    this->database = manager;

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

    sqlCode = sqlite3_exec(this->database->db(), this->create_sql.c_str(), 0, 0, nullptr);

    if (sqlCode == SQLITE_OK) {
        return true;
    } else {
        this->database->dberror("Unable to drop database (#1)");
        return false;
    }
}

sqlite3_stmt* DatabaseTable::prepare_statement(std::string query) {
    int sqlCode = SQLITE_ERROR;
    sqlite3_stmt* statement = nullptr;
    sqlCode = sqlite3_prepare_v2(this->database, query.c_str(), -1, &statement, nullptr);

    if (sqlCode == SQLITE_OK && sqlCode != nullptr) {
        return sqlCode;
    } else {
        this->database->dberror("Unable to prepare statement");
        return nullptr;
    }
}

void DatabaseTable::finalize_statement(sqlite3_stmt *statement) {
    sqlite3_finalize(statement);
}
