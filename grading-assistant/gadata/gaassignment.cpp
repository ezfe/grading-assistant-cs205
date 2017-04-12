#include "gaassignment.h"

/*!
 * \brief Deconstruct the GAAssignment
 *
 * Deconstructs constituent rubrics
 */
GAAssignment::~GAAssignment() {
    delete this->rubric;
}

/*!
 * \brief Get the title
 * \return The title
 */
std::string GAAssignment::get_title() {
    return this->title;
}

/*!
 * \brief Set the title
 * \param title The title
 */
void GAAssignment::set_title(std::string title) {
    this->title = title;
}

/*!
 * \brief Get the description
 * \return The description
 */
std::string GAAssignment::get_description() {
    return this->description;
}

/*!
 * \brief Set the description
 * \param description The description
 */
void GAAssignment::set_description(std::string description) {
    this->description = description;
}

/*!
 * \brief Get the class this assignment is in
 * \return The class
 */
GAClass* GAAssignment::get_class() {
    return this->class_;
}

/*!
 * \brief Set the class
 *
 * Do not call this method directly, instead use the GAClass methods to add it.
 *
 * \param class_ The class
 */
void GAAssignment::set_class(GAClass* class_) {
    this->class_ = class_;
}

/*!
 * \brief Get the rubric for this assignment
 * \return The rubric
 */
GARubric* GAAssignment::get_rubric() {
    return this->rubric;
}

/*!
 * \brief Set the rubric for this assignment
 *
 * Important: rubrics are managed by the GradingAssistant object so you must add them to that object as well.
 * This will not do that for you!!!
 *
 * \param rubric The rubric
 */
void GAAssignment::set_rubric(GARubric* rubric) {
    this->rubric = rubric;
}

/*!
 * \brief Save this assignment to a table
 * \param table The table
 * \return The table
 */
bool GAAssignment::save_to(DatabaseTable* table) {
    if (this->class_ == nullptr || this->rubric == nullptr) {
        //Don't save assignments not attached to a class
        //Also, currently require a rubric. May be changed in the future?
        return false;
    }

    std::string values = DatabaseTable::escape_string(this->get_id()) + ", ";
    values += DatabaseTable::escape_string(this->title) + ", ";
    values += DatabaseTable::escape_string(this->description) + ", ";
    values += DatabaseTable::escape_string(this->class_->get_id()) + ", ";
    values += DatabaseTable::escape_string(this->rubric->get_id());

    return table->insert("id, title, description, class, rubric", values);
}

/*!
 * \brief Load assignments from a table which are in a certain class
 * \param table The table
 * \param class_ The class
 * \return The list of assignments
 */
std::vector<GAAssignment*> GAAssignment::load_from(DatabaseTable* assignmentTable, DatabaseTable* rubricTable, DatabaseTable* rubricRowTable, DatabaseTable* rubricRowValuesTable, GAClass* class_) {
    std::vector<GAAssignment*> found;
    sqlite3_stmt* statement = assignmentTable->prepare_statement(assignmentTable->prepare_select_all("class = " + DatabaseTable::escape_string(class_->get_id())));
    while(sqlite3_step(statement) == SQLITE_ROW) {
        GAAssignment* assignment = new GAAssignment(assignmentTable->get_string(statement, 0));
        assignment->set_title(assignmentTable->get_string(statement, 1));
        assignment->set_description(assignmentTable->get_string(statement, 2));
        assignment->set_class(class_);

        GARubric* rubric = GARubric::load_from(rubricTable, rubricRowTable, rubricRowValuesTable, DatabaseTable::get_string(statement, 4));
        assignment->set_rubric(rubric);

        found.push_back(assignment);
    }
    assignmentTable->finalize_statement(statement);
    return found;
}
