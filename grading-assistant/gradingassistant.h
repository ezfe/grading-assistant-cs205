#ifndef GRADINGASSISTANT_H
#define GRADINGASSISTANT_H

#include <vector>
#include "gadata/gaclass.h"
#include "databasemanager.h"
#include "databasetable.h"

class GradingAssistant {
public:
    GradingAssistant();
    ~GradingAssistant();

    std::vector<GAClass*> get_classes();
    void add_class(GAClass* c);

    std::string to_string();

    bool save(DatabaseManager* database);
private:
    std::vector<GAClass*> classes;
};

#endif // GRADINGASSISTANT_H
