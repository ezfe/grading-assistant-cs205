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
    GAIdentifiableObject(std::string id);
    virtual ~GAIdentifiableObject();

    std::string get_id();
    void set_id(std::string id);
private:
    std::string id = "{00000000-0000-0000-0000-000000000000}";
};

#endif // GAIDENTIFIABLEOBJECT_H
