#include "databasemanager.h"

/*!
 * \brief Create a DatabaseManager
 *
 * This method should not be called
 */
DatabaseManager::DatabaseManager() {
    this->active = false;
}

/*!
 * \brief Create a DatabaseManager
 *
 * The Database will be opened automatically. Re-calling open() is ok.
 *
 * \param path The path used to store the database
 */
DatabaseManager::DatabaseManager(std::string path) {
    this->current = nullptr;
    this->path = path;
    this->open();
}

/*!
 * \brief Deconstruct the DatabaseManager
 *
 * This will close the database. It is ok to close() it before deconstruction.
 */
DatabaseManager::~DatabaseManager() {
    this->close();
}

/*!
 * \brief Get the path used to store the database
 * \return The path
 */
std::string DatabaseManager::get_path() {
    return this->path;
}

/*!
 * \brief Set the path used to store the database
 *
 * This method will automatically close() and re open() the database. It is
 * safe to call this on a closed database.
 *
 * \param path
 */
void DatabaseManager::set_path(std::string path) {
    this->close();
    this->path = path;
    this->open();
}

/*!
 * \brief Open the database, if it is not already opened
 */
void DatabaseManager::open() {
    if (this->is_open() || !this->active) {
        return;
    }

    int sqlCode = sqlite3_open(this->path.c_str(), &this->current);

    if (sqlCode != SQLITE_OK) {
        this->dberror("Unable to open database");
    }
}

/*!
 * \brief Close the database, if it is already opened
 */
void DatabaseManager::close() {
    if (!this->is_open() || !this->active) {
        return;
    }

    int sqlCode = sqlite3_close(this->current);

    if (sqlCode == SQLITE_OK) {
        this->current = nullptr;
    } else {
        this->dberror("Unable to close database");
    }
}

/*!
 * \brief Check if the database is open
 * \return Whether the database is open or closed
 */
bool DatabaseManager::is_open() {
    return current ? true : false;
}

/*!
 * \brief Get the sqlite3 database reference
 * \return The sqlite3 database reference
 */
sqlite3* DatabaseManager::db() {
    return current;
}

/*!
 * \brief Print the sqlite3 error to cerr
 */
void DatabaseManager::dberror() {
    std::cerr << "Database Error: " << sqlite3_errmsg(current) << std::endl;
}

/*!
 * \brief Print a message, and the sqlite3 error to cerr
 * \param message The message to print in addition to the sqlite3 error
 */
void DatabaseManager::dberror(std::string message) {
    std::cerr << message << std::endl;
    this->dberror();
}

bool DatabaseManager::is_active() {
    return this->active;
}
