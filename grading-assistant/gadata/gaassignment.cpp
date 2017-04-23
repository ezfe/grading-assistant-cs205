
#include "gaassignment.h"

/*!
 * \brief Deconstruct the GAAssignment
 *
 * Deconstructs constituent rubrics
 */
GAAssignment::~GAAssignment() {
    delete this->rubric;
    this->rubric = nullptr;
}

/*!
 * \brief Get the title
 * \return The title
 */
std::string GAAssignment::get_title() {
    return this->title;
    this->rubric = nullptr;
}

/*!
 * \brief Set the title
 * \param title The title
 */
void GAAssignment::set_title(std::string title, bool save) {
    this->title = title;
    if (save) {
        this->save(false);
    }
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
void GAAssignment::set_description(std::string description, bool save) {
    this->description = description;
    if (save) {
        this->save(false);
    }
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
void GAAssignment::set_class(GAClass* class_, bool save) {
    if (this->class_ != nullptr) {
        //        delete this->class_;
        //        this->class_ = nullptr;
    }
    this->class_ = class_;
    if (save) {
        this->save(false);
    }
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
void GAAssignment::set_rubric(GARubric* rubric, bool save) {
    if (rubric == nullptr) {
        std::cerr << "Receveid nullptr rubric" << std::endl;
        return;
    }
    if (this->rubric != nullptr) {
        delete this->rubric;
        this->rubric = nullptr;
    }
    this->rubric = rubric;
    this->rubric->set_grading_assistant(this->get_grading_assistant());
    if (save) {
        this->save(false);
    }
}

/*!
 * \brief Save this assignment to a table
 * \param table The table
 * \return The table
 */
void GAAssignment::save(bool cascade) {
    std::cout << "Starting save for GAAssignment " << this->get_title() << std::endl;
    std::cout << "Cascade: " << (cascade ? "yes" : "no") << std::endl;

    if (this->get_grading_assistant() == nullptr) {
        std::cout << "No grading assistant, aborting save" << std::endl;
        return;
    }
    if (this->get_class() == nullptr) {
        std::cout << "- No class, not saving" << std::endl;
        return;
    }
    if (this->get_rubric() == nullptr) {
        std::cout << "- No rubric, not saving" << std::endl;
        return;
    }

    std::string values = DatabaseTable::escape_string(this->get_id()) + ", ";
    values += DatabaseTable::escape_string(this->title) + ", ";
    values += DatabaseTable::escape_string(this->description) + ", ";
    values += DatabaseTable::escape_string(this->class_->get_id()) + ", ";
    values += DatabaseTable::escape_string(this->rubric->get_id());

    this->get_grading_assistant()->assignmentTable->insert("id, title, description, class, rubric", values);
    if (cascade) {
        this->get_rubric()->save(true);
    }
}

/*!
 * \brief Remove this object from the table
 *
 * Will remove the rubric as well
 *
 * \return Whether the delete was successfull
 */
bool GAAssignment::remove() {
    bool anyFail = false;
    anyFail = !this->get_grading_assistant()->assignmentTable->delete_row_wid(this->get_id()) || anyFail;

    QDir dir(QString::fromStdString(FileManager::get_assignment_directory(this)));
    dir.removeRecursively();

    anyFail = !this->rubric->remove() || anyFail;
    delete this->rubric;
    this->rubric = nullptr;

    return anyFail;
}

/*!
 * \brief Load assignments from a table which are in a certain class
 * \param table The table
 * \param class_ The class
 * \return The list of assignments
 */
std::vector<GAAssignment*> GAAssignment::load(GradingAssistant* ga, GAClass* class_) {
    DatabaseTable* assignmentTable = ga->assignmentTable;

    std::vector<GAAssignment*> found;
    sqlite3_stmt* statement = assignmentTable->prepare_statement(assignmentTable->prepare_select_all("class = " + DatabaseTable::escape_string(class_->get_id())));
    while(sqlite3_step(statement) == SQLITE_ROW) {
        GAAssignment* assignment = new GAAssignment(assignmentTable->get_string(statement, 0), ga);
        GARubric* rubric = GARubric::load(ga, DatabaseTable::get_string(statement, 4));
        assignment->set_rubric(rubric, false);
        assignment->set_class(class_, false);

        assignment->set_title(assignmentTable->get_string(statement, 1), false);
        assignment->set_description(assignmentTable->get_string(statement, 2), false);

        found.push_back(assignment);
    }
    assignmentTable->finalize_statement(statement);
    return found;
}
