#ifndef GASTUDENT_H
#define GASTUDENT_H

#include <iostream>
#include <string>
#include <map>

#include "gaidentifiableobject.h"
#include "gaclass.h"
#include "gaassignment.h"
#include "gaassignmentdata.h"

class GAClass;

class GAStudent: public GAIdentifiableObject {
public:
    using GAIdentifiableObject::GAIdentifiableObject;
    GAStudent(std::string name, std::string laf_id);

    virtual ~GAStudent();

    std::string get_name();
    void set_name(std::string name);

    std::string get_lafayette_username();
    void set_lafayette_username(std::string username);

    GAClass* get_class();
    void set_class(GAClass* class_);

    void set_data(GAAssignment* a, GAAssignmentData* d);
    GAAssignmentData* get_data(GAAssignment* a);
    std::map<GAAssignment*, GAAssignmentData*> get_map();

    virtual bool save_to(DatabaseTable* table);

    std::string to_string();
private:
    std::string name;
    std::string lafayette_username;
    GAClass* class_ = nullptr;
    std::map<GAAssignment*, GAAssignmentData*> assignmentData;
};

#endif // GASTUDENT_H
