#include "class.h"

Class::Class() {

}

Class::~Class() {
    for(Student* student: this->students) {
        delete student;
    }

    for(Assignment* assignment: this->assignments) {
        delete assignment;
    }
}

std::vector<Student*> Class::get_students() {
    return this->students;
}

void Class::add_student(Student* student) {
    this->students.push_back(student);
}

std::vector<Assignment*> Class::get_assignments() {
    return this->assignments;
}

void Class::add_assignment(Assignment *assignment) {
    this->assignments.push_back(assignment);
}
