#ifndef GAASSIGNMENTDATA_H
#define GAASSIGNMENTDATA_H

#include <vector>

#include "gaassignment.h"
#include "gaannotation.h"
#include "gaidentifiableobject.h"

class GAAssignment;
class GAAnnotation;
class GAStudent;

class GAAssignmentData: public GAIdentifiableObject {
public:
    using GAIdentifiableObject::GAIdentifiableObject;

    virtual ~GAAssignmentData();

    GAAssignment* get_assignment();
    void set_assignment(GAAssignment * a);

    GAStudent* get_student();
    void set_student(GAStudent * a);

    void add_annotation(GAAnnotation* a);

    std::vector<GAAnnotation*> get_comments();
    std::vector<GAAnnotation*> get_problems();
    std::vector<GAAnnotation*> get_extra_credit();
    std::vector<GAAnnotation*> get_by_type(std::string type);
    std::vector<GAAnnotation*> get_annotations();

    bool save_to(DatabaseTable* table);
    static GAAssignmentData* load_from(DatabaseTable* table, GAAssignment* assignment, GAStudent* student);
private:
    GAAssignment* assignment;
    GAStudent* student;
    std::vector<GAAnnotation*> annotations;
};

#endif // GAASSIGNMENTDATA_H
