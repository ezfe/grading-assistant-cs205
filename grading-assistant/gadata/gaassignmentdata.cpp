#include "gaassignmentdata.h"

GAAssignmentData::~GAAssignmentData() {
    /* This class owns GAAnnotations */
    /* Nothing else has to be managed here */

    for(GAAnnotation* assign: this->comments) {
        delete assign;
    }
    this->comments.clear();;

    for(GAAnnotation* assign: this->problems) {
        delete assign;
    }
    this->problems.clear();;

    for(GAAnnotation* assign: this->extraCredit) {
        delete assign;
    }
    this->extraCredit.clear();;
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
    if(a->get_type() == "comment") {
        comments.push_back(a);
    } else if(a->get_type() == "problem") {
        problems.push_back(a);
    } else {
        extraCredit.push_back(a);
    }
}

std::vector<GAAnnotation*> GAAssignmentData::get_comments() {
    return comments;
}

std::vector<GAAnnotation*> GAAssignmentData::get_problems() {
    return problems;
}

std::vector<GAAnnotation*> GAAssignmentData::get_extra_credit() {
    return extraCredit;
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
