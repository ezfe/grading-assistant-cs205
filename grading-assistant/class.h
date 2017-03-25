#ifndef CLASS_H
#define CLASS_H

#include <iostream>
#include <string>
#include <vector>

#include "student.h"
#include "assignment.h"

class Class {
public:
    Class();
    ~Class();

    std::string get_name();
    void set_name(std::string name);

    std::vector<Student*> get_students();
    void add_student(Student* student);

    std::vector<Assignment*> get_assignments();
    void add_assignment(Assignment* assignment);

    std::string to_string();
private:
    std::string name;
    std::vector<Student*> students;
    std::vector<Assignment*> assignments;
    /* store user assignment data here as well? */
};

#endif // CLASS_H
