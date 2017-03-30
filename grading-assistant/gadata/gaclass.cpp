#include "gaclass.h"

GAClass::~GAClass() {
    for(GAStudent* student: this->students) {
        delete student;
    }

    for(GAAssignment* assignment: this->assignments) {
        delete assignment;
    }
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
}

std::vector<GAAssignment*> GAClass::get_assignments() {
    return this->assignments;
}

void GAClass::add_assignment(GAAssignment *assignment) {
    this->assignments.push_back(assignment);
}

bool GAClass::save_to(DatabaseTable* table) {
    return table->insert("id, name", this->id_string() + ", \"" + this->name + "\"");
}

std::string GAClass::to_string() {
    return "Class{" + this->get_name() + "}";
}
