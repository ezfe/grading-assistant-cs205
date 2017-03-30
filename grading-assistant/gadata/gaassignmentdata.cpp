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
    return false;
}

