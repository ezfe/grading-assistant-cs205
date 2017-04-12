#include "gaassignment.h"

/*!
 * \brief Deconstruct the GAAssignment
 *
 * Currently does nothing
 */
GAAssignment::~GAAssignment() {
    /* This class currently owns nothing */

    /* The rubrics are managed by the GradingAssistant class */
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
    if (this->rubric == nullptr) {
        /* try to resolve a promise, if we don't have a rubric */
        /* this may not do anything if the object doesn't have a rubric */
        this->resolve_promise();
    }
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
 * \brief Set the ID of a future rubric
 * \param p The persistence ID
 */
void GAAssignment::set_rubric_promise(std::string p) {
    if (this->rubric == nullptr) {
        this->__rubric_promise = p;
    } else {
        std::cerr << "You should not set rubric identifier after loading a rubric" << std::endl;
    }
}

/*!
 * \brief Will attempt to resolve a promised ID to a memory pointer
 * \param ga
 */
void GAAssignment::resolve_promise() {
    if (this->__rubric_promise != "" && this->rubric == nullptr) {
        GradingAssistant* ga = this->get_class()->get_grading_assistant();
        this->rubric = ga->get_rubric(this->__rubric_promise);
    }
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
std::vector<GAAssignment*> GAAssignment::load_from(DatabaseTable* table, GAClass* class_) {
    std::vector<GAAssignment*> found;
    sqlite3_stmt* statement = table->prepare_statement(table->prepare_select_all("class = " + DatabaseTable::escape_string(class_->get_id())));
    while(sqlite3_step(statement) == SQLITE_ROW) {
        GAAssignment* assignment = new GAAssignment(table->get_string(statement, 0));
        assignment->set_title(table->get_string(statement, 1));
        assignment->set_description(table->get_string(statement, 2));
        assignment->set_class(class_);

        /* This must be resolved later on */
        assignment->set_rubric_promise(table->get_string(statement, 4));

        found.push_back(assignment);
    }
    table->finalize_statement(statement);
    return found;
}
