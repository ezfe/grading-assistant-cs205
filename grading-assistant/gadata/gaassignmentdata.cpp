#include "gaassignmentdata.h"

GAAssignmentData::GAAssignmentData()
{

}

GAAssignment* GAAssignmentData::get_assignment() {
    return assignment;
}

void GAAssignmentData::set_assignment(GAAssignment *a) {
    assignment = a;
}

void GAAssignmentData::add_annotation(GAAnnotation *a) {
    if(a->get_type() == "comment")
    {
        comments.push_back(a);
    }
    else if(a->get_type() == "problem")
    {
        problems.push_back(a);
    }
    else
    {
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
