#ifndef GAIDENTIFIABLEOBJECT_H
#define GAIDENTIFIABLEOBJECT_H

#include <iostream>
#include <sqlite3.h>

#include "databasetable.h"

class GAIdentifiableObject {
public:
    GAIdentifiableObject();
    GAIdentifiableObject(int id);

    int id = -1;

    virtual bool save_to(DatabaseTable* table) = 0;

    std::string id_string();
};

#endif // GAIDENTIFIABLEOBJECT_H
