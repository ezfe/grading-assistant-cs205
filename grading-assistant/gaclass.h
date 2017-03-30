#ifndef GACLASS_H
#define GACLASS_H

#include <iostream>
#include <string>
#include <vector>

#include "gastudent.h"
#include "gaassignment.h"
#include "gaidentifiableobject.h"
#include "databasetable.h"

class GAClass: public GAIdentifiableObject {
public:
    using GAIdentifiableObject::GAIdentifiableObject;

    ~GAClass();

    std::string get_name();
    void set_name(std::string name);

    std::vector<GAStudent*> get_students();
    void add_student(GAStudent* student);

    std::vector<GAAssignment*> get_assignments();
    void add_assignment(GAAssignment* assignment);

    virtual bool save_to(DatabaseTable* table);

    std::string to_string();
private:
    std::string name;
    std::vector<GAStudent*> students;
    std::vector<GAAssignment*> assignments;
    /* store user assignment data here as well? */
};

#endif // GACLASS_H
