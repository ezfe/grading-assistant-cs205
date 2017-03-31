#ifndef GRADINGASSISTANT_H
#define GRADINGASSISTANT_H

#include <vector>
#include "gadata/gaclass.h"
#include "databasemanager.h"
#include "databasetable.h"

class GradingAssistant {
public:
    GradingAssistant(DatabaseManager* database);
    ~GradingAssistant();

    std::vector<GAClass*> get_classes();
    void add_class(GAClass* c);

    std::string to_string();

    bool save();
    bool load();
private:
    GradingAssistant();

    std::vector<GAClass*> classes;

    DatabaseManager* database;

    DatabaseTable* annotationTable;
    DatabaseTable* assignmentTable;
    DatabaseTable* assignmentDataTable;
    DatabaseTable* classesTable;
    DatabaseTable* rubricTable;
    DatabaseTable* rubricRowTable;
    DatabaseTable* studentTable;
};

#endif // GRADINGASSISTANT_H
