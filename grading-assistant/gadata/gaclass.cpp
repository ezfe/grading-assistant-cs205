#include "gaclass.h"

GAClass::GAClass(std::string name): GAIdentifiableObject() {
    this->name = name;
}

GAClass::~GAClass() {
    /* This object owns students and assignments */

    for(GAStudent* student: this->students) {
        delete student;
    }
    this->students.clear();

    for(GAAssignment* assignment: this->assignments) {
        delete assignment;
    }
    this->assignments.clear();
}

std::string GAClass::get_name() {
    return this->name;
}

void GAClass::set_name(std::string name) {
    this->name = name;
}

std::vector<GAStudent*> GAClass::get_students() {
    return this->students;
}

void GAClass::add_student(GAStudent* student) {
    this->students.push_back(student);
    student->set_class(this);
}

std::vector<GAAssignment*> GAClass::get_assignments() {
    return this->assignments;
}

void GAClass::add_assignment(GAAssignment *assignment) {
    this->assignments.push_back(assignment);
    assignment->set_class(this);
}

bool GAClass::save_to(DatabaseTable* table) {
    std::string values = DatabaseTable::escape_string(this->id_string()) + ", ";
    values += DatabaseTable::escape_string(this->name);
    return table->insert("id, name", values);
}

std::string GAClass::to_string() {
    return "Class{" + this->get_name() + "}";
}
