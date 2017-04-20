#ifndef GACLASS_H
#define GACLASS_H

#include <iostream>
#include <string>
#include <vector>

#include "gaidentifiableobject.h"
#include "gastudent.h"
#include "gaassignment.h"
#include "../databasetable.h"

class GAStudent;
class GAAssignment;

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
    void remove_student(GAStudent* student);

    std::vector<GAAssignment*> get_assignments();
    void add_assignment(GAAssignment* assignment);
    void remove_assignment(GAAssignment* assignment);

    bool save(bool cascade);
    virtual bool remove();
    static std::vector<GAClass*> load(GradingAssistant *ga);
private:
    std::string name;

    std::vector<GAStudent*> students;
    std::vector<GAAssignment*> assignments;
};

#endif // GACLASS_H
