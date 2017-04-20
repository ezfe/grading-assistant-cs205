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
    a->set_grading_assistant(this->get_grading_assistant());
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

std::vector<GAAnnotation*> GAAssignmentData::get_by_category(std::string category) {
    std::vector<GAAnnotation*> found;
    for(GAAnnotation* a: this->annotations) {
        if(a->get_category() == category) {
            found.push_back(a);
        }
    }
    return found;
}

/*!
 * @brief Get the line numbers of all annotations in the given file
 * @param fileName
 * @return vector of line numbers
 */
std::vector<int> GAAssignmentData::get_line_numbers(std::string fileName) {
    std::vector<int> lines;
    for(GAAnnotation *a : annotations) {
        if(a->get_filename() == fileName) {
            lines.push_back(a->get_line());
        }
    }
    std::sort(lines.begin(), lines.end());
    return lines;
}

/*!
 * @brief Gets a specific annotation given a location
 * @param fileName of annotation
 * @param lineNumber of annotation
 * @return GAAnnotation at the given location
 */
GAAnnotation* GAAssignmentData::get_annotation(std::string fileName, int lineNumber) {
    for(GAAnnotation *a : annotations) {
        if(a->get_filename() == fileName && a->get_line() == lineNumber) {
            return a;
        }
    }
    return nullptr;
}

/*!
 * \brief Calculate the score for this assignment
 *
 * This will return the number of *points* the student has earned. Use calculate_percentage() for a percentage;
 *
 * \return The score
 */
int GAAssignmentData::calculate_score() {
    if (this->manual_score >= 0) {
        return this->manual_score;
    } else {
        if (this->assignment->get_rubric() == nullptr) {
            std::cerr << "No rubric for grading" << std::endl;
            return 0;
        } else {
            GARubric* rubric = this->get_assignment()->get_rubric();
            int score = 0;
            for(GARubricRow* row: rubric->get_rows()) {
                score += this->calculate_score(row);
            }
            if(rubric->get_ec() != nullptr) {
                score += this->calculate_score(rubric->get_ec());
            }
            return score;
        }
    }
}

/*!
 * \brief Calculate the score for a rubric row
 * \param for_row The rubric row
 * \return The score
 */
int GAAssignmentData::calculate_score(GARubricRow* for_row) {
    std::string cat = for_row->get_category();
    int score;
    if(for_row->is_extra_credit()) {
        score = 0;
    }
    else {
        score = for_row->get_max_points();
    }
    for(GAAnnotation* annot: this->annotations) {
        if (annot->get_category() == cat) {
            score += annot->get_points();
        }
    }
    if (score > 0) {
        return score;
    }
    return 0;
}

/*!
 * \brief Override the score for this assignment
 * \param manual The number of *points* this studen thas earned
 */
void GAAssignmentData::override_score(int manual) {
    if (manual >= 0) {
        this->manual_score = manual;
    } else {
        this->reset_score();
    }
}

/*!
 * \brief Reset the override on this assignment
 *
 * This will default to regular score calculation
 */
void GAAssignmentData::reset_score() {
    this->manual_score = -1;
}

/*!
 * \brief Check whether the score provided by calculate_score() is a manually overriden score
 * \return Whether it is an overriden score
 */
bool GAAssignmentData::is_overriden() {
    return (this->manual_score >= 0);
}

/*!
 * \brief Fetch a list of files (by path) for this GAAssignmentData
 * \return The list of file paths
 */
std::vector<std::string> GAAssignmentData::query_files() {
    std::string root = FileManager::get_assignment_student_directory(this);

    std::vector<std::string> return_value;
    return_value.push_back(FileManager::append(root, "testfile.java"));
    return_value.push_back(FileManager::append(root, "testfile2.java"));
    return_value.push_back(FileManager::append(root, "testfile3.java"));
    return return_value;
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
        values += DatabaseTable::escape_string(assignment->get_id()) + ", ";
        values += std::to_string(this->manual_score);
        return table->insert("id, student, assignment, manual_score", values);
    }
}

/*!
 * \brief Remove this object from the table
 * \return Whether the delete was successful
 */
bool GAAssignmentData::remove() {
    std::cerr << "AssignmentData remove() Unimplemented" << std::endl;
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
        found->override_score(table->get_int(statement, 3)); //if negative, will be set to not overriden
    }
    table->finalize_statement(statement);
    return found;
}
