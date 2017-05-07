#include "databasetable.h"

/*!
 * \brief Create a DatabaseTable
 *
 * This method should not be called
 */
DatabaseTable::DatabaseTable() {
    std::cerr << "Must provide parameters" << std::endl;
}

/*!
 * \brief Create a DatabaseTable
 * \param manager The DatabaseManager being used
 * \param name The name of the table
 * \param schema The schema of the table
 */
DatabaseTable::DatabaseTable(DatabaseManager *manager, std::string name, std::string schema) {
    this->name = name;
    this->create_sql = "CREATE TABLE IF NOT EXISTS " + this->name + " (" + schema + ");";
    this->database = manager;

    this->create();
}

/*!
 * \brief Drop the table
 *
 * If the table doesn't exist, nothing will happen.
 *
 * \return Whether the table was dropped
 */
bool DatabaseTable::drop() {
    return this->single_exec("DROP TABLE IF EXISTS " + this->name + ";") == SQLITE_OK;
}

/*!
 * \brief Create the table
 *
 * If the table already exists, even if with a different schema, nothing will happen.
 * This means new schemas *must* be dropped and recreated.
 *
 * \return Whether the table was created
 */
bool DatabaseTable::create() {
    return this->single_exec(this->create_sql) == SQLITE_OK;
}

bool DatabaseTable::recreate() {
    bool dropped = this->drop();
    bool created = this->create();
    return dropped && created;
}

/*!
 * \brief Get the name of the table
 * \return The name of the table
 */
std::string DatabaseTable::get_name() {
    return this->name;
}

/*!
 * \brief Create a query selecting columns under certain conditions
 *
 * To select all columns, use prepare_select_all(std::string where)
 *
 * \param schema The columns to fetch, comma seperated
 * \param where The condition the row must meet
 * \return The query string
 */
std::string DatabaseTable::prepare_query(std::string schema, std::string where) {
    std::string sortpart = "";
    if (this->is_sorted()) {
        sortpart = "ORDER BY " + this->get_sort();
    }
    return "SELECT " + schema + " FROM " + this->name + " WHERE " + where + " " + sortpart + ";";
}

/*!
 * \brief Create a query selecting columns
 *
 * To select all columns, use prepare_select_all()
 *
 * \param schema The columns to fetch, comma seperated
 * \return The query string
 */
std::string DatabaseTable::prepare_query(std::string schema) {
    std::string sortpart = "";
    if (this->is_sorted()) {
        sortpart = "ORDER BY " + this->get_sort();
    }
    return "SELECT " + schema + " FROM " + this->name + " " + sortpart + ";";
}

/*!
 * \brief Create a query selecting all columns under certain conditions
 * \param where The columns to fetch, comma seperated
 * \return The query string
 */
std::string DatabaseTable::prepare_select_all(std::string where) {
    return this->prepare_query("*", where);
}

/*!
 * \brief Create a query selecting all rows and columns
 * \return The query string
 */
std::string DatabaseTable::prepare_select_all() {
    return this->prepare_query("*");
}

/*!
 * \brief Create an insert statement
 *
 * You should probably use insert(std::string columns, std::string values)
 *
 * \param columns The columns to insert into
 * \param values The values to insert
 * \return The query string
 */
std::string DatabaseTable::prepare_insert(std::string columns, std::string values) {
    return "INSERT OR REPLACE INTO " + this->name +" (" + columns + ") VALUES (" + values +");";
}

/*!
 * \brief Insert into the table
 * \param columns The columns to insert into
 * \param values The values to insert
 * \return Whether the insert was successful
 */
bool DatabaseTable::insert(std::string columns, std::string values) {
    std::string query = this->prepare_insert(columns, values);
    int code = this->single_exec(query);
    return (code == SQLITE_DONE);
}

/*!
 * \brief Prepare a statement
 * \param query The query string
 * \return The sqlite3_stmt object
 */
sqlite3_stmt* DatabaseTable::prepare_statement(std::string query) {
    int sqlCode = SQLITE_ERROR;
    sqlite3_stmt* statement = nullptr;
    sqlCode = sqlite3_prepare_v2(this->database->db(), query.c_str(), -1, &statement, nullptr);

    if (sqlCode == SQLITE_OK && statement != nullptr) {
        return statement;
    } else {
        this->database->dberror("Unable to prepare statement: " + query);
        return nullptr;
    }
}

/*!
 * \brief Get an int from the table
 * \param statement The sqlite3_stmt object
 * \param column The column
 * \return The int
 */
int DatabaseTable::get_int(sqlite3_stmt* statement, int column) {
    return sqlite3_column_int(statement, column);
}

/*!
 * \brief Get a string from the table
 * \param statement The sqlite3_stmt object
 * \param column The column
 * \return  The string
 */
std::string DatabaseTable::get_string(sqlite3_stmt* statement, int column) {
    const char* temp = reinterpret_cast<const char*>(sqlite3_column_text(statement, column));
    return temp == NULL ? "null" : std::string(temp);
}

/*!
 * \brief Finalize the sqlite3_stmt
 *
 * This must be called to indicate you are finished with the sqlite3_stmt object
 *
 * \param statement The sqlite3_stmt object
 */
void DatabaseTable::finalize_statement(sqlite3_stmt *statement) {
    sqlite3_finalize(statement);
}

/*!
 * \brief Execute a query once
 * \param query The query string
 * \return The sqlite3 return code
 */
int DatabaseTable::single_exec(std::string query) {
    if (!this->database->is_active()) return SQLITE_OK;

    sqlite3_stmt* statement = this->prepare_statement(query);

    int sqlCode = SQLITE_ERROR;
    if (statement != nullptr) {
        sqlCode = sqlite3_step(statement);
    }

    this->finalize_statement(statement);
    return sqlCode;
}

/*!
 * \brief Delete a row from the database based on the ID column
 * \param id The ID
 * \return ==SQLITE_DONE
 */
bool DatabaseTable::delete_row_wid(std::string id) {
    return delete_row("id = " + DatabaseTable::escape_string(id));
}

/*!
 * \brief delete a row from the database
 * \param where The condition
 * \return ==SQLITE_DONE
 */
bool DatabaseTable::delete_row(std::string where) {
    return this->single_exec("DELETE FROM " + this->get_name() + " WHERE " + where) == SQLITE_DONE;
}


/*!
 * \brief Put quotes around a string for usage in the database
 *
 * `Hello` -> `"Hello"`
 *
 * `How are "you" doing` -> `'How are "you" doing'`
 *
 * \param string The string to process
 * \return The enquoted string
 */
std::string DatabaseTable::escape_string(std::string string) {
    if (string.find('"') == std::string::npos) {
        return "\"" + string + "\"";
    } else if (string.find('\'') == std::string::npos) {
        return "'" + string + "'";
    } else {
        std::cerr << "Unable to escape string: " << string << std::endl;
        return string;
    }
}

std::string DatabaseTable::get_sort() {
    return this->sort_str;
}

bool DatabaseTable::is_sorted() {
    return this->sort;
}

void DatabaseTable::set_sort(std::string sort) {
    this->sort_str = sort;
    this->sort = true;
}

void DatabaseTable::unset_sort() {
    this->sort_str = "";
    this->sort = false;
}
