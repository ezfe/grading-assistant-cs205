#ifndef GAASSIGNMENTDATA_H
#define GAASSIGNMENTDATA_H

#include <vector>

#include "gaidentifiableobject.h"
#include "gaannotation.h"
#include "gaassignment.h"
#include "garubricrow.h"
#include "../filemanager.h"

class GAStudent;
class DatabaseTable;
class GAAnnotation;
class GAAssignment;

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
    std::vector<GAAnnotation*> get_by_category(std::string category);
    std::vector<GAAnnotation*> get_annotations();

    int calculate_score();
    int calculate_score(GARubricRow* for_row);
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
