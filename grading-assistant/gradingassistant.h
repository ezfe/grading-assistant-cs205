#ifndef GRADINGASSISTANT_H
#define GRADINGASSISTANT_H

#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include "gadata/gaclass.h"
#include "databasemanager.h"
#include "databasetable.h"
#include "gadata/garubric.h"

class GAClass;
class GAAnnotation;

class GradingAssistant {
public:
    GradingAssistant(DatabaseManager* database);
    ~GradingAssistant();

    std::vector<GAClass*> get_classes();
    GAClass* get_class(std::string identifier);
    void add_class(GAClass* c);
    void remove_class(GAClass* c);

    std::vector<GARubric*> get_rubrics();
    GARubric* get_rubric(std::string identifier);
    void add_rubric(GARubric* r);
    void remove_rubric(GARubric* r);

    std::vector<GAAnnotation*> query_annotation(std::string search);

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
