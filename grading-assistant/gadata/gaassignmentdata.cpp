#include "gaassignmentdata.h"

/*!
 * \brief Deconstruct the assignment data
 *
 * Deconstruct the annotations inside this container
 */
GAAssignmentData::~GAAssignmentData() {
    /* This class owns GAAnnotations */
    /* Nothing else has to be managed here */

    for(GAAnnotation* annot: this->annotations) {
        delete annot;
    }
    this->annotations.clear();;
}

/*!
 * \brief Get the assignment
 * \return The assignment
 */
GAAssignment* GAAssignmentData::get_assignment() {
    return this->assignment;
}

/*!
 * \brief Set the assignment
 * \param a The assignment
 */
void GAAssignmentData::set_assignment(GAAssignment* a) {
    this->assignment = a;
}

/*!
 * \brief Get the student
 * \return The student
 */
GAStudent* GAAssignmentData::get_student() {
    return this->student;
}

/*!
 * \brief Set the student
 * \param s The student
 */
void GAAssignmentData::set_student(GAStudent* s) {
    this->student = s;
}

/*!
 * \brief Add an annotation
 * \param a The annotation
 */
void GAAssignmentData::add_annotation(GAAnnotation* a) {
    this->annotations.push_back(a);
    a->set_assignment_data(this);
}

/*!
 * \brief Get the comments
 * \return The list of comments
 */
std::vector<GAAnnotation*> GAAssignmentData::get_comments() {
    return get_by_type(GA_ANNOTATION_COMMENT);
}

/*!
 * \brief Get the problems [-points]
 * \return The list of problems
 */
std::vector<GAAnnotation*> GAAssignmentData::get_problems() {
    return get_by_type(GA_ANNOTATION_PROBLEM);
}

/*!
 * \brief Get the extra credits [+points]
 * \return The list of extra credits
 */
std::vector<GAAnnotation*> GAAssignmentData::get_extra_credit() {
    return get_by_type(GA_ANNOTATION_EXTRACREDIT);
}

/*!
 * \brief Get the annotations with a certain type
 *
 * You should not call this directly. Instead call
 *
 * - get_extra_credit()
 * - get_problems()
 * - get_comments()
 *
 * \param type The type
 * \return The list of annotations
 */
std::vector<GAAnnotation*> GAAssignmentData::get_by_type(std::string type) {
    std::vector<GAAnnotation*> found;
    for(GAAnnotation* a: this->annotations) {
        if (a->get_type() == type) {
            found.push_back(a);
        }
    }
    return found;
}

/*!
 * \brief Get all the annotations
 * \return The annotations
 */
std::vector<GAAnnotation*> GAAssignmentData::get_annotations() {
    return this->annotations;
}

/*!
 * \brief Save the assignment data to a table.
 *
 * This will not save constituent annotations
 *
 * This will abort if it does not have either an assignment or class
 *
 * \param table The table
 * \return Whether the insert was successful
 */
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

/*!
 * \brief Load a single GAAssignmentData from a table
 * \param table The table
 * \param assignment The assignment
 * \param student The student
 * \return The assignment data object
 */
GAAssignmentData* GAAssignmentData::load_from(DatabaseTable* table, GAAssignment* assignment, GAStudent* student) {
    GAAssignmentData* found = nullptr;
    std::string where = "assignment = " + DatabaseTable::escape_string(assignment->get_id()) + " AND ";
    where += "student = " + DatabaseTable::escape_string(student->get_id());
    sqlite3_stmt* statement = table->prepare_statement(table->prepare_select_all(where));
    if (sqlite3_step(statement) == SQLITE_ROW) {
        found = new GAAssignmentData(table->get_string(statement, 0));
        found->set_student(student);
        found->set_assignment(assignment);
    }
    table->finalize_statement(statement);
    return found;
}
