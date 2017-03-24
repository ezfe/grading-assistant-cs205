#include "class.h"

Class::Class() {

}

Class::~Class() {
    for(Student* student: this->students) {
        delete student;
    }
}

std::vector<Student*> Class::get_students() {
    return this->students;
}

void Class::add_student(Student* student) {
    this->students.push_back(student);
}
