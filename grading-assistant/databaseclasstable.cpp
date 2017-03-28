#include "databaseclasstable.h"

std::string DatabaseClassTable::create_sql() {
    return "CREATE TABLE IF NOT EXISTS " + this->name + "(id INTEGER, name TEXT);";
}
