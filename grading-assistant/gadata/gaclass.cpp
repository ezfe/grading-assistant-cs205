#include "gaclass.h"

GAClass::GAClass(std::string name): GAIdentifiableObject() {
    this->name = name;
}

GAClass::GAClass(std::string id, std::string name): GAIdentifiableObject(id) {
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

void GAClass::remove_student(GAStudent* delete_student) {
    std::vector<GAStudent*> current = this->students;
    this->students.clear();
    for(GAStudent* check_student: current) {
        if (check_student != delete_student) {
            this->students.push_back(check_student);
        }
    }
    delete delete_student;
}

std::vector<GAAssignment*> GAClass::get_assignments() {
    return this->assignments;
}

void GAClass::add_assignment(GAAssignment *assignment) {
    this->assignments.push_back(assignment);
    assignment->set_class(this);
}

bool GAClass::save_to(DatabaseTable* table) {
    std::string values = DatabaseTable::escape_string(this->get_id()) + ", ";
    values += DatabaseTable::escape_string(this->name);
    return table->insert("id, name", values);
}

std::vector<GAClass*> GAClass::load_from(DatabaseTable* table) {
    std::vector<GAClass*> found;
    sqlite3_stmt* statement = table->prepare_statement(table->prepare_select_all());
    while(sqlite3_step(statement) == SQLITE_ROW) {
        GAClass* c = new GAClass(table->get_string(statement, 0), table->get_string(statement, 1));
        found.push_back(c);
    }
    table->finalize_statement(statement);
    return found;
}

std::string GAClass::to_string() {
    return "Class{" + this->get_name() + "}";
}
