#ifndef GRADINGASSISTANT_H
#define GRADINGASSISTANT_H

#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

#include "databasetable.h"
#include "gadata/gaclass.h"
#include "gadata/garubric.h"
#include "gadata/gastudent.h"
#include "gadata/gaassignmentdata.h"
#include "gadata/gaannotation.h"
#include "gadata/garubricrow.h"
#include "gadata/gaassignment.h"

class DatabaseManager;

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
