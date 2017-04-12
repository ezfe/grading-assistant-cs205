#ifndef GACLASS_H
#define GACLASS_H

#include <iostream>
#include <string>
#include <vector>

#include "gastudent.h"
#include "gaassignment.h"
#include "gaidentifiableobject.h"
#include "../databasetable.h"
#include "../gradingassistant.h"

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
    void remove_student(GAStudent* student);

    std::vector<GAAssignment*> get_assignments();
    void add_assignment(GAAssignment* assignment);
    void remove_assignment(GAAssignment* assignment);

    GradingAssistant* get_grading_assistant();
    void set_grading_assistant(GradingAssistant* ga);

    bool save_to(DatabaseTable* table);
    static std::vector<GAClass*> load_from(DatabaseTable* table);
private:
    std::string name;

    std::vector<GAStudent*> students;
    std::vector<GAAssignment*> assignments;

    GradingAssistant* grading_assistant;
};

#endif // GACLASS_H
