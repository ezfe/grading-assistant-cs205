#include "gaassignmentdata.h"

GAAssignmentData::~GAAssignmentData() {
    /* This class owns GAAnnotations */
    /* Nothing else has to be managed here */

    for(GAAnnotation* annot: this->annotations) {
        delete annot;
    }
    this->annotations.clear();;
}

GAAssignment* GAAssignmentData::get_assignment() {
    return this->assignment;
}

void GAAssignmentData::set_assignment(GAAssignment* a) {
    this->assignment = a;
}

GAStudent* GAAssignmentData::get_student() {
    return this->student;
}

void GAAssignmentData::set_student(GAStudent* s) {
    this->student = s;
}

void GAAssignmentData::add_annotation(GAAnnotation* a) {
    this->annotations.push_back(a);
    a->set_assignment_data(this);
}

std::vector<GAAnnotation*> GAAssignmentData::get_comments() {
    return get_by_type(GA_ANNOTATION_COMMENT);
}

std::vector<GAAnnotation*> GAAssignmentData::get_problems() {
    return get_by_type(GA_ANNOTATION_PROBLEM);
}

std::vector<GAAnnotation*> GAAssignmentData::get_extra_credit() {
    return get_by_type(GA_ANNOTATION_EXTRACREDIT);
}

std::vector<GAAnnotation*> GAAssignmentData::get_by_type(std::string type) {
    std::vector<GAAnnotation*> found;
    for(GAAnnotation* a: this->annotations) {
        if (a->get_type() == type) {
            found.push_back(a);
        }
    }
    return found;
}

std::vector<GAAnnotation*> GAAssignmentData::get_annotations() {
    return this->annotations;
}

bool GAAssignmentData::save_to(DatabaseTable* table) {
    if (this->assignment == nullptr || this->student == nullptr) {
        return false;
    } else {
        std::string values = DatabaseTable::escape_string(this->get_id()) + ", ";
        values += DatabaseTable::escape_string(student->get_id()) + ", ";
        values += DatabaseTable::escape_string(assignment->get_id());
        return table->insert("id, student, assignment", values);
    }
}

GAAssignmentData* GAAssignmentData::load_from(DatabaseTable* table, GAAssignment* assignment, GAStudent* student) {
    GAAssignmentData* found = nullptr;
    std::string where = "assignment = " + DatabaseTable::escape_string(assignment->get_id()) + " AND ";
    where += "student = " + DatabaseTable::escape_string(student->get_id());
    sqlite3_stmt* statement = table->prepare_statement(table->prepare_select_all(where));
    while(sqlite3_step(statement) == SQLITE_ROW) {
        found = new GAAssignmentData(table->get_string(statement, 0));
        found->set_student(student);
        found->set_assignment(assignment);
    }
    table->finalize_statement(statement);
    return found;
}
