#ifndef GACLASS_H
#define GACLASS_H

#include <iostream>
#include <string>
#include <vector>

#include "gastudent.h"
#include "gaassignment.h"
#include "gaidentifiableobject.h"
#include "databasetable.h"

class GAStudent;

class GAClass: public GAIdentifiableObject {
public:
    using GAIdentifiableObject::GAIdentifiableObject;
    GAClass(std::string name);
    GAClass(std::string id, std::string name);

    virtual ~GAClass();

    std::string get_name();
    void set_name(std::string name);

    std::vector<GAStudent*> get_students();
    void add_student(GAStudent* student);

    std::vector<GAAssignment*> get_assignments();
    void add_assignment(GAAssignment* assignment);

    bool save_to(DatabaseTable* table);
    static std::vector<GAClass*> load_from(DatabaseTable* table);

    std::string to_string();
private:
    std::string name;

    std::vector<GAStudent*> students;
    std::vector<GAAssignment*> assignments;
};

#endif // GACLASS_H
