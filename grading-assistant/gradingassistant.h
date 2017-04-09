#ifndef GRADINGASSISTANT_H
#define GRADINGASSISTANT_H

#include <vector>
#include "gadata/gaclass.h"
#include "databasemanager.h"
#include "databasetable.h"
#include "gadata/garubric.h"

class GradingAssistant {
public:
    GradingAssistant(DatabaseManager* database);
    ~GradingAssistant();

    std::vector<GAClass*> get_classes();
    void add_class(GAClass* c);

    std::vector<GARubric*> get_rubrics();
    void add_rubric(GARubric* r);

    void save();
    void load();
private:
    GradingAssistant();

    std::vector<GAClass*> classes;
    std::vector<GARubric*> rubrics;

    DatabaseManager* database;

    DatabaseTable* annotationTable;
    DatabaseTable* assignmentTable;
    DatabaseTable* assignmentDataTable;
    DatabaseTable* classesTable;
    DatabaseTable* rubricTable;
    DatabaseTable* rubricRowTable;
    DatabaseTable* rubricRowValuesTable;
    DatabaseTable* studentTable;
};

#endif // GRADINGASSISTANT_H
