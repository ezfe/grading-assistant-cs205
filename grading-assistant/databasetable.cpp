#include "databasetable.h"

DatabaseTable::DatabaseTable() {
    std::cerr << "Must provide parameters" << std::endl;
}

DatabaseTable::DatabaseTable(DatabaseManager *manager, std::string name, std::string schema) {
    this->name = name;
    this->create_sql = "CREATE TABLE IF NOT EXISTS " + this->name + " (" + schema + ");";
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

std::string DatabaseTable::prepare_query(std::string schema, std::string where) {
    return "SELECT " + schema + " FROM " + this->name + " WHERE " + where + ";";
}

std::string DatabaseTable::prepare_query(std::string schema) {
    return "SELECT " + schema + " FROM " + this->name + ";";
}

std::string DatabaseTable::prepare_select_all(std::string where) {
    return this->prepare_query("*", where);
}

std::string DatabaseTable::prepare_select_all() {
    return this->prepare_query("*");
}

std::string DatabaseTable::prepare_insert(std::string columns, std::string values) {
    return "INSERT INTO " + this->name +" (" + columns + ") VALUES (" + values +");";
}

bool DatabaseTable::insert(std::string columns, std::string values) {
    std::string query = this->prepare_insert(columns, values);
    int code = this->single_exec(query);
    return (code == SQLITE_DONE);
}


sqlite3_stmt* DatabaseTable::prepare_statement(std::string query) {
    int sqlCode = SQLITE_ERROR;
    sqlite3_stmt* statement = nullptr;
    sqlCode = sqlite3_prepare_v2(this->database->db(), query.c_str(), -1, &statement, nullptr);

    if (sqlCode == SQLITE_OK && statement != nullptr) {
        return statement;
    } else {
        this->database->dberror("Unable to prepare statement");
        return nullptr;
    }
}

void DatabaseTable::finalize_statement(sqlite3_stmt *statement) {
    sqlite3_finalize(statement);
}

int DatabaseTable::single_exec(std::string query) {
    sqlite3_stmt* statement = this->prepare_statement(query);

    int sqlCode = SQLITE_ERROR;
    if (statement != nullptr) {
        sqlCode = sqlite3_step(statement);
    }

    this->finalize_statement(statement);
    return sqlCode;
}
