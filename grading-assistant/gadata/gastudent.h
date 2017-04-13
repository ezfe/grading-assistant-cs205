#ifndef GASTUDENT_H
#define GASTUDENT_H

#include <iostream>
#include <string>
#include <map>

#include "gadata/gaidentifiableobject.h"
#include "gadata/gaassignmentdata.h"
#include "gadata/gaassignment.h"

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
    GAAssignmentData* get_data(GAAssignment* a);
    std::map<GAAssignment*, GAAssignmentData*> get_map();

    bool save_to(DatabaseTable* table);
    void remove_from(DatabaseTable* table);
    static std::vector<GAStudent*> load_from(DatabaseTable* table, GAClass* class_);
private:
    std::string name;
    std::string lafayette_username;
    GAClass* class_ = nullptr;
    std::map<GAAssignment*, GAAssignmentData*> assignmentData;
};

#endif // GASTUDENT_H
