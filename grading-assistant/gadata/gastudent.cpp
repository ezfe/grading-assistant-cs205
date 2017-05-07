#include "gastudent.h"

/*!
 * \brief Construct a GAStudent
 * \param name The name for the student
 * \param laf_id The Lafayette username for the student (not their student ID)
 */
GAStudent::GAStudent(std::string name, std::string laf_id): GAIdentifiableObject() {
    this->name = name;
    this->lafayette_username = laf_id;
}

/*!
 * \brief Construct a GAStudent with a provided persistence ID
 * \param id The ID to use
 * \param name The name for the student
 * \param laf_id The Lafayette username for the student (not their student ID)
 */
GAStudent::GAStudent(std::string id, std::string name, std::string laf_id): GAIdentifiableObject(id) {
    this->name = name;
    this->lafayette_username = laf_id;
}

/*!
 * \brief Deconstruct a GAStudent
 *
 * This will remove all the AssignmentData objects, which are owned by the GAStudent objects
 */
GAStudent::~GAStudent() {
    /* This class currently owns GAAssignmentData */
    for (auto const& x: this->assignmentData) {
        delete x.second;
    }
}

/*!
 * \brief Get the name of the student
 * \return The name of the student
 */
std::string GAStudent::get_name() {
    return this->name;
}

/*!
 * \brief Set the name of the student
 * \param name The name of the student
 */
void GAStudent::set_name(std::string name) {
    this->name = name;
    this->save(false);
}

/*!
 * \brief Get the Lafayette username of the student
 * \return The Lafayette username
 */
std::string GAStudent::get_lafayette_username() {
    return this->lafayette_username;
}

/*!
 * \brief Set the Lafayette username of the student
 * \param username The Lafayette username
 */
void GAStudent::set_lafayette_username(std::string username) {
    this->lafayette_username = username;
    this->save(false);
}

/*!
 * \brief Get the GAClass the student is enrolled in
 * \return The class
 */
GAClass* GAStudent::get_class() {
    return this->class_;
}

/*!
 * \brief Set the GAClass the student is enrolled in
 * \param class_ The class
 */
void GAStudent::set_class(GAClass* class_) {
    this->class_ = class_;
    this->save(false);
}

/*!
 * \brief Set the assignment data object
 *
 * Do not create an assignment data object, instead call get_data(GAAssignmentData* a) to get
 * a properly linked object.
 *
 * \param a The GAAssignment
 * \param d The GAAssignmentData
 */
void GAStudent::set_data(GAAssignment* a, GAAssignmentData* d) {
    d->set_grading_assistant(this->get_grading_assistant());
    delete this->assignmentData[a];
    this->assignmentData[a] = d;
    d->set_assignment(a);
    d->set_student(this);
    this->save(true);
}

/*!
 * \brief Remove a GAAssignmentData object from the map
 * \param a Assignment data object
 */
void GAStudent::remove_data(GAAssignment* a) {
    this->assignmentData[a]->remove();
    delete this->assignmentData[a];
    this->assignmentData.erase(a);
}

/*!
 * \brief Get the assignment data object
 *
 * This will not return nullptr unless the student is not in the class the assignment is associated with
 *
 * \param a The assignment
 * \return The assignment data
 */
GAAssignmentData* GAStudent::get_data(GAAssignment* a) {
    if (a->get_class() != this->get_class()) {
        std::cerr << "This student has no data for that assignment" << std::endl;
        return nullptr;
    }
    if (this->assignmentData[a] == nullptr) {
        GAAssignmentData* d = new GAAssignmentData();
        d->set_grading_assistant(this->get_grading_assistant());
        this->set_data(a, d);
    }
    return this->assignmentData[a];
}

/*!
 * \brief Get the GAAssignment - GAAssignmentData map
 * \return The map
 */
std::map<GAAssignment*, GAAssignmentData*> GAStudent::get_map() {
    return this->assignmentData;
}


/*!
 * @brief Calculate the student's average lab grade
 * @return average grade
 */
int GAStudent::calculate_lab_grade() {
    int sum = 0;
    if(this->get_map().size() == 0) {
        return 100;
    }
    else {
        for (auto const& x: this->get_map()) {
            sum += x.second->calculate_percentage();
        }
        return ceil((double)sum/this->get_map().size());
    }
}

/*!
 * \brief Save this object to a table
 * \param cascade Whether to save the assignment data and annotations
 * \return Whether the insert was successful
 */
void GAStudent::save(bool cascade) {
    /*
     * If we dont' have a grading assistant or class,
     * we cannot save the student
     */
    if (this->get_grading_assistant() == nullptr)   return;
    if (this->class_ == nullptr)                    return;

    std::string escaped_id = DatabaseTable::escape_string(this->get_id());
    std::string escaped_name = DatabaseTable::escape_string(this->name);
    std::string escaped_laf = DatabaseTable::escape_string(this->lafayette_username);
    std::string escaped_class_id = DatabaseTable::escape_string(this->class_->get_id());
    std::string values = escaped_id + ", " + escaped_name + ", " + escaped_laf + ", " + escaped_class_id;
    this->get_grading_assistant()->studentTable->insert("id, name, lafayette_username, class", values);

    if (cascade) {
        /* Loop through the assignment data objects */
        for (auto const& x: this->get_map()) {
            /* Save the assignment data to the table */
            x.second->save(true);
        }
    }
}

/*!
 * \brief Remove this object from the table
 *
 * Will remove all of the assignment data objects (todo)
 *
 * \return Whether the delete was successful
 */
bool GAStudent::remove() {
    bool anyFail = !this->get_grading_assistant()->studentTable->delete_row_wid(this->get_id());

    for (auto const& x: this->assignmentData) {
        anyFail = !x.second->remove() || anyFail;
        delete x.second;
    }
    this->assignmentData.clear();

    return anyFail;
}

/*!
 * \brief Load all students from a table (who are enrolled in a specific class)
 * \param ga The grading assistant
 * \param class_ The class
 * \return The student vector
 */
std::vector<GAStudent*> GAStudent::load(GradingAssistant* ga, GAClass* class_) {
    DatabaseTable* table = ga->studentTable;
    std::vector<GAStudent*> found;

    if (!table->is_active()) return found;

    sqlite3_stmt* statement = table->prepare_statement(table->prepare_select_all("class = " + DatabaseTable::escape_string(class_->get_id())));
    while(sqlite3_step(statement) == SQLITE_ROW) {
        GAStudent* s = new GAStudent(table->get_string(statement, 0), ga);
        s->set_name(table->get_string(statement, 1));
        s->set_lafayette_username(table->get_string(statement, 2));
        found.push_back(s);
    }
    table->finalize_statement(statement);
    return found;
}
