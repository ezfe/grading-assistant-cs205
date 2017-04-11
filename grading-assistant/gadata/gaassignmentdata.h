#ifndef GAASSIGNMENTDATA_H
#define GAASSIGNMENTDATA_H

#include <vector>

#include "gaassignment.h"
#include "gaannotation.h"
#include "gaidentifiableobject.h"
#include "../filemanager.h"

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

    int calculate_score();
    void override_score(int manual);
    void reset_score();
    bool is_overriden();

    std::vector<std::string> query_files();

    bool save_to(DatabaseTable* table);
    static GAAssignmentData* load_from(DatabaseTable* table, GAAssignment* assignment, GAStudent* student);
private:
    GAAssignment* assignment;
    GAStudent* student;
    std::vector<GAAnnotation*> annotations;

    /*!
     * \brief Manually overriden score for the assignment
     *
     * Negative numbers will be ignored and the score will be calculated based on the rows in the rubric
     */
    int manual_score = -1;
};

#endif // GAASSIGNMENTDATA_H
