#ifndef DATABASECLASSTABLE_H
#define DATABASECLASSTABLE_H

#include <string>

#include "databasetable.h"

class DatabaseClassTable : public DatabaseTable {
public:
    using DatabaseTable::DatabaseTable;

    std::string create_sql();
};

#endif // DATABASECLASSTABLE_H
