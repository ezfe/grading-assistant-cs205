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
class GAAnnotation;
class GAStudent;
class GAClass;
class GARubric;

/*!
 * \brief The main grading assistant
 */
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

    GAStudent* get_student(std::string identifier);

    std::vector<GAAnnotation*> query_annotation(std::string search);

    std::vector<std::string> get_years();
    std::vector<GAClass*> get_by_info(std::string semester, std::string year);

    void save();
    void load();

    DatabaseTable* annotationTable;
    DatabaseTable* assignmentTable;
    DatabaseTable* assignmentDataTable;
    DatabaseTable* classesTable;
    DatabaseTable* rubricTable;
    DatabaseTable* rubricRowTable;
    DatabaseTable* rubricRowValuesTable;
    DatabaseTable* studentTable;
private:
    GradingAssistant();

    std::vector<GAClass*> classes;

    DatabaseManager* database;
};

#endif // GRADINGASSISTANT_H
