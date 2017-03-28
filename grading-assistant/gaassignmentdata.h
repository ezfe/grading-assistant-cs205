#ifndef GAASSIGNMENTDATA_H
#define GAASSIGNMENTDATA_H

#include <vector>

#include "gaassignment.h"
#include "gaannotation.h"

class GAAssignmentData
{
public:
    GAAssignmentData();

    GAAssignment* get_assignment();
    void set_assignment(GAAssignment * a);

    void add_annotation(GAAnnotation* a);

    std::vector<GAAnnotation*> get_comments();

    std::vector<GAAnnotation*> get_problems();

    std::vector<GAAnnotation*> get_extra_credit();

private:
    GAAssignment * assignment;
    std::vector<GAAnnotation*> comments;
    std::vector<GAAnnotation*> problems;
    std::vector<GAAnnotation*> extraCredit;
};

#endif // GAASSIGNMENTDATA_H