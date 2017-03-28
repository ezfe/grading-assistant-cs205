#include "databasemanager.h"

DatabaseManager::DatabaseManager() {
    std::cerr << "No path provided" << std::endl;
}

DatabaseManager::DatabaseManager(std::string path) {
    this->current = nullptr;
    this->path = path;
    this->open();
}

DatabaseManager::~DatabaseManager() {
    this->close();
}

std::string DatabaseManager::get_path() {
    return this->path;
}

void DatabaseManager::set_path(std::string path) {
    this->close();
    this->path = path;
    this->open();
}

void DatabaseManager::open() {
    if (this->is_open()) {
        return;
    }

    int sqlCode = sqlite3_open(this->path.c_str(), &this->current);

    if (sqlCode != SQLITE_OK) {
        this->dberror("Unable to open database");
    }
}

void DatabaseManager::close() {
    if (!this->is_open()) {
        return;
    }

    int sqlCode = sqlite3_close(this->current);

    if (sqlCode == SQLITE_OK) {
        this->current = nullptr;
    } else {
        this->dberror("Unable to close database");
    }
}

bool DatabaseManager::is_open() {
    return current ? true : false;
}

sqlite3* DatabaseManager::db() {
    return current;
}

void DatabaseManager::dberror() {
    std::cerr << "Database Error: " << sqlite3_errmsg(current) << std::endl;
}

void DatabaseManager::dberror(std::string message) {
    std::cerr << message << std::endl;
    this->dberror();
}
