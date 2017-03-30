#ifndef GAIDENTIFIABLEOBJECT_H
#define GAIDENTIFIABLEOBJECT_H

#include <iostream>
#include <string>
#include <sqlite3.h>
#include <QUuid>

#include "databasetable.h"

class GAIdentifiableObject {
public:
    GAIdentifiableObject();


    virtual bool save_to(DatabaseTable* table) = 0;

    std::string id_string();
private:
    std::string id = "{00000000-0000-0000-0000-000000000000}";
};

#endif // GAIDENTIFIABLEOBJECT_H
