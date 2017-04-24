#ifndef GASTUDENT_H
#define GASTUDENT_H

#include <iostream>
#include <string>
#include <map>
#include <cmath>

#include "gaidentifiableobject.h"
#include "gaassignmentdata.h"
#include "gaassignment.h"

class GAClass;
class GAAssignment;
class GAAssignmentData;
class DatabaseTable;

class GAStudent: public GAIdentifiableObject {
public:
    using GAIdentifiableObject::GAIdentifiableObject;
    GAStudent(std::string name, std::string laf_id);
    GAStudent(std::string id, std::string name, std::string laf_id);

    virtual ~GAStudent();

    std::string get_name();
    void set_name(std::string name);

    std::string get_lafayette_username();
    void set_lafayette_username(std::string username);

    GAClass* get_class();
    void set_class(GAClass* class_);

    void set_data(GAAssignment* a, GAAssignmentData* d);
    void remove_data(GAAssignment* a);
    GAAssignmentData* get_data(GAAssignment* a);
    std::map<GAAssignment*, GAAssignmentData*> get_map();

    int calculate_lab_grade();

    bool save(bool cascade);
    virtual bool remove();
    static std::vector<GAStudent*> load(GradingAssistant* ga, GAClass* class_);
private:
    std::string name;
    std::string lafayette_username;
    GAClass* class_ = nullptr;
    std::map<GAAssignment*, GAAssignmentData*> assignmentData;
};

#endif // GASTUDENT_H
