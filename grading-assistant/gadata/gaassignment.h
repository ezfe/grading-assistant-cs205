#ifndef GAASSIGNMENT_H
#define GAASSIGNMENT_H

#include <iostream>
#include <string>

#include "gadata/gaidentifiableobject.h"
#include "databasetable.h"
#include "gadata/gaclass.h"
#include "gadata/garubric.h"

class GAClass;

class GAAssignment: public GAIdentifiableObject {
public:
    using GAIdentifiableObject::GAIdentifiableObject;
    virtual ~GAAssignment();

    std::string get_title();
    void set_title(std::string title);

    std::string get_description();
    void set_description(std::string description);

    GAClass* get_class();
    void set_class(GAClass* class_);

    GARubric* get_rubric();
    void set_rubric(GARubric* rubric);

    bool save_to(DatabaseTable* table);
    static std::vector<GAAssignment*> load_from(DatabaseTable* assignmentTable, DatabaseTable* rubricTable, DatabaseTable* rubricRowTable, DatabaseTable* rubricRowValuesTable, GAClass* class_);
private:
    std::string title;
    std::string description;

    GAClass* class_ = nullptr;
    GARubric* rubric = nullptr;
};

#endif // GAASSIGNMENT_H
