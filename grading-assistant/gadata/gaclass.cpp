#include "gaclass.h"

/*!
 * \brief Construct a GAClass with a name
 * \param name The name
 */
GAClass::GAClass(std::string name): GAIdentifiableObject() {
    this->name = name;
}

/*!
 * \brief Construct a GAClass with a provided persistence ID and name
 * \param id The persistence ID
 * \param name The name
 */
GAClass::GAClass(std::string id, std::string name): GAIdentifiableObject(id) {
    this->name = name;
}

/*!
 * \brief Deconstruct a GAClass
 *
 * This will deconstruct the students and assignments that are
 * part of this class.
 */
GAClass::~GAClass() {
    /* This object owns students and assignments */

    for(GAStudent* student: this->students) {
        delete student;
    }

    for(GAAssignment* assignment: this->assignments) {
        delete assignment;
    }
}

/*!
 * \brief Get the name of the class
 * \return The name
 */
std::string GAClass::get_name() {
    return this->name;
}

/*!
 * \brief Set the name of the class
 * \param name The name
 */
void GAClass::set_name(std::string name) {
    this->name = name;
    this->save(false);
}

/*!
 * \brief Get a list of students in the class
 * \return The list of students
 */
std::vector<GAStudent*> GAClass::get_students() {
    return this->students;
}

/*!
 * \brief Add a student to the class
 * \param student The student
 */
void GAClass::add_student(GAStudent* student) {
    this->students.push_back(student);
    student->set_class(this);
    student->set_grading_assistant(this->get_grading_assistant());
    student->save(true);
}

/*!
 * \brief Remove a student from the class
 *
 * This will remove the student from the class (and the database)
 *
 * \param delete_student The student
 */
void GAClass::remove_student(GAStudent* delete_student) {
    std::vector<GAStudent*> current = this->students;
    this->students.clear();
    for(GAStudent* check_student: current) {
        if (check_student != delete_student) {
            this->students.push_back(check_student);
        }
    }
    delete_student->remove();
    delete delete_student;
}

/*!
 * \brief Get a list of assignments for this class
 * \return The list of assignments
 */
std::vector<GAAssignment*> GAClass::get_assignments() {
    return this->assignments;
}

/*!
 * \brief Add an assignment to the class
 * \param assignment The assignment
 */
void GAClass::add_assignment(GAAssignment *assignment) {
    this->assignments.push_back(assignment);
    assignment->set_class(this);
    assignment->set_grading_assistant(this->get_grading_assistant());
    assignment->save(true);
}

/*!
 * \brief Remove an assignment from the class
 *
 * This will remove the assignment from memory
 *
 * \param assignment The assignment
 */
void GAClass::remove_assignment(GAAssignment *assignment) {
    //Remove the assignment data first
    for(GAStudent* student: this->students) {
        student->remove_data(assignment);
    }

    std::vector<GAAssignment*> current = this->assignments;
    this->assignments.clear();
    for(GAAssignment* ass: current) {
        if (ass != assignment) {
            this->assignments.push_back(ass);
        }
    }
    assignment->remove();
    delete assignment;
}

/*!
 * \brief Save this object to a table
 * \param cascade Whether to save all the constituent objects
 * \return Whether the table was inserted properly
 */
bool GAClass::save(bool cascade) {
    std::cout << "Starting save for GAClass " << this->get_name() << std::endl;
    std::cout << "Cascade: " << (cascade ? "yes" : "no") << std::endl;

    if (this->get_grading_assistant() == nullptr) {
        std::cout << "No grading assistant, not saving" << std::endl;
        return false;
    }

    DatabaseTable* table = this->get_grading_assistant()->classesTable;

    std::string values = DatabaseTable::escape_string(this->get_id()) + ", ";
    values += DatabaseTable::escape_string(this->name);
    bool inserted = table->insert("id, name", values);

    if (cascade) {
        /* Loop through the assignments */
        for(GAAssignment* a: this->get_assignments()) {
            a->save(true);
        }

        /* Loop through the students */
        for(GAStudent* s: this->get_students()) {
            s->save(true);
        }
    }
    return inserted;
}

/*!
 * \brief Removes the class and all its constituent objects from the database
 * \return Successful
 */
bool GAClass::remove() {
    bool anyDidFail = false;

    anyDidFail = !this->get_grading_assistant()->classesTable->delete_row_wid(this->get_id()) || anyDidFail;

    QDir dir(QString::fromStdString(FileManager::get_class_directory(this)));
    dir.removeRecursively();

    /*!
      Not calling the remove_xxx() functions because those are for removing single
      items and preserving the class itself. When we're nuking everything its more performant
      to do it directly
      */

    for(GAStudent* student: this->students) {
        anyDidFail = !student->remove() || anyDidFail;
        delete student;
    }
    this->students.clear();

    for(GAAssignment* assignment: this->assignments) {
        anyDidFail = !assignment->remove() || anyDidFail;
        delete assignment;
    }
    this->assignments.clear();

    return !anyDidFail;
}

/*!
 * \brief Load all the classes from a table
 * \param table The table
 * \return The list of classes
 */
std::vector<GAClass*> GAClass::load(GradingAssistant* ga) {
    DatabaseTable* table = ga->classesTable;

    std::vector<GAClass*> found;
    sqlite3_stmt* statement = table->prepare_statement(table->prepare_select_all());
    while(sqlite3_step(statement) == SQLITE_ROW) {
        GAClass* c = new GAClass(table->get_string(statement, 0), table->get_string(statement, 1));
        found.push_back(c);
    }
    table->finalize_statement(statement);
    return found;
}
