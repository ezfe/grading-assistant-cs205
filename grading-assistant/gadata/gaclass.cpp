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
    this->students.clear();

    for(GAAssignment* assignment: this->assignments) {
        delete assignment;
    }
    this->assignments.clear();
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
}

/*!
 * \brief Remove a student from the class
 *
 * This will remove the student from memory
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
}

/*!
 * \brief Remove an assignment from the class
 *
 * This will remove the assignment from memory
 *
 * \param assignment The assignment
 */
void GAClass::remove_assignment(GAAssignment *assignment) {
    //TODO: Implement
}

/*!
 * \brief Save this class to a table
 * \param table The table
 * \return Whether the insert was successful
 */
bool GAClass::save_to(DatabaseTable* table) {
    std::string values = DatabaseTable::escape_string(this->get_id()) + ", ";
    values += DatabaseTable::escape_string(this->name);
    return table->insert("id, name", values);
}

/*!
 * \brief Load all the classes from a table
 * \param table The table
 * \return The list of classes
 */
std::vector<GAClass*> GAClass::load_from(DatabaseTable* table) {
    std::vector<GAClass*> found;
    sqlite3_stmt* statement = table->prepare_statement(table->prepare_select_all());
    while(sqlite3_step(statement) == SQLITE_ROW) {
        GAClass* c = new GAClass(table->get_string(statement, 0), table->get_string(statement, 1));
        found.push_back(c);
    }
    table->finalize_statement(statement);
    return found;
}
