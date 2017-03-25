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

    std::vector<Student*> get_students();
    void add_student(Student* student);

    std::vector<Assignment*> get_assignments();
    void add_assignment(Assignment* assignment);

private:
    std::string name;
    std::vector<Student*> students;
    std::vector<Assignment*> assignments;
};

#endif // CLASS_H
