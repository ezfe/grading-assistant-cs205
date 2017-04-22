#include "gaannotation.h"

/*!
 * \brief Construct a GAAnnotation with a type
 * \param type The type
 */
GAAnnotation::GAAnnotation(std::string type): GAIdentifiableObject() {
    this->set_type(type);
}

/*!
 * \brief Construct a GAAnnotation with a persistence ID and type
 * \param id The persistence ID
 * \param type The type
 */
GAAnnotation::GAAnnotation(std::string id, std::string type): GAIdentifiableObject(id) {
    this->set_type(type);
}

/*!
 * \brief Construct a GAAnnotation with a persistence ID, type, and title
 * \param id The persistence ID
 * \param type The type
 * \param title The title
 */
GAAnnotation::GAAnnotation(std::string id, std::string type, std::string title): GAIdentifiableObject(id) {
    this->set_type(type);
    this->title = title;
}

/*!
 * \brief Deconstruct the GAAnnotation
 *
 * Nothing currently happens here
 */
GAAnnotation::~GAAnnotation() {
    /* This class currently owns nothing */
}

/*!
 * \brief Get the title
 * \return The title
 */
std::string GAAnnotation::get_title() {
    return this->title;
}

/*!
 * \brief Set the title
 * \param title The title
 */
void GAAnnotation::set_title(std::string title) {
    this->title = title;
    this->save();
}

/*!
 * \brief Get the type
 * \return The type
 */
std::string GAAnnotation::get_type() {
    return this->type;
}

/*!
 * \brief Set the type
 * \param type The type
 */
void GAAnnotation::set_type(std::string type) {
    if (type == GA_ANNOTATION_COMMENT || type == GA_ANNOTATION_EXTRACREDIT || type == GA_ANNOTATION_PROBLEM) {
        this->type = type;
        this->category = type;
    } else {
        this->type = type;
        this->category = GA_ANNOTATION_UNSET;
    }
    this->save();
}

/*!
 * \brief Get the description
 * \return The description
 */
std::string GAAnnotation::get_description() {
    return this->description;
}

/*!
 * \brief Set the description
 * \param description The description
 */
void GAAnnotation::set_description(std::string description) {
    this->description = description;
    this->save();
}

/*!
 * \brief Get the category
 * \return The category
 */
std::string GAAnnotation::get_category() {
    return this->category;
}

/*!
 * \brief Set the category
 * \param category The category
 */
void GAAnnotation::set_category(std::string category) {
    this->category = category;
    this->save();
}

/*!
 * \brief Get the filename
 * \return The filename
 */
std::string GAAnnotation::get_filename() {
    return this->filename;
}

/*!
 * \brief Set the filename
 * \param location The filename
 */
void GAAnnotation::set_filename(std::string filename) {
    this->filename = filename;
    this->save();
}

/*!
 * \brief Get the line
 * \return The line
 */
int GAAnnotation::get_line() {
    return this->line;
}

/*!
 * \brief Set the line
 * \param line The line
 */
void GAAnnotation::set_line(int line) {
    this->line = line;
    this->save();
}

std::string GAAnnotation::get_location() {
    return this->get_filename() + "@" + std::to_string(this->get_line());
}

/*!
 * \brief Get the assignment data object
 * \return The assignment data object
 */
GAAssignmentData* GAAnnotation::get_assignment_data() {
    return this->data;
}

/*!
 * \brief Set the assignment data object
 * \param data The assignment data object
 */
void GAAnnotation::set_assignment_data(GAAssignmentData *data) {
    this->data = data;
    this->save();
}

/*!
 * \brief Get the point value
 *
 * This can be zero, positive, or negative
 *
 * \return The point value
 */
int GAAnnotation::get_points() {
    return this->value;
}

/*!
 * \brief Set the point value
 * \param value The point value
 */
void GAAnnotation::set_points(int value) {
    this->value = value;
    this->save();
}

/*!
 * \brief Copy the annotation
 * \return The copy
 */
GAAnnotation* GAAnnotation::copy() {
    GAAnnotation* newa = new GAAnnotation(this->get_grading_assistant());
    newa->set_category(this->get_category());
    newa->set_description(this->get_description());
    newa->set_filename(this->get_filename());
    newa->set_line(this->get_line());
    newa->set_points(this->get_points());
    newa->set_title(this->get_title());
    newa->set_type(this->get_type());
    return newa;
}

/*!
 * \brief Save the annotation to a table
 * \param table The table
 * \return Whether the insert was successful
 */
bool GAAnnotation::save() {
    std::cout << "Starting save for GAAnnotation " << this->get_title() << std::endl;

    if (this->get_grading_assistant() == nullptr) {
        std::cout << "- No grading assistant, not saving" << std::endl;
        return false;
    }
    if (this->get_assignment_data() == nullptr) {
        std::cout << "- No assignment data, not saving" << std::endl;
        return false;
    }

    std::string values = DatabaseTable::escape_string(this->get_id()) + ", ";
    values += DatabaseTable::escape_string(this->get_assignment_data()->get_id()) + ", ";
    values += DatabaseTable::escape_string(this->get_type()) + ", ";
    values += DatabaseTable::escape_string(this->get_title()) + ", ";
    values += DatabaseTable::escape_string(this->get_description()) + ", ";
    values += DatabaseTable::escape_string(this->get_category()) + ", ";
    values += std::to_string(this->get_points()) + ", ";
    values += DatabaseTable::escape_string(this->get_filename()) + ", ";
    values += std::to_string(this->get_line());
    return this->get_grading_assistant()->annotationTable->insert("id, assignment_data, type, title, description, category, points, filename, line", values);
}

/*!
 * \brief Remove this object from the table
 * \return Whether the delete was successful
 */
bool GAAnnotation::remove() {
    return this->get_grading_assistant()->annotationTable->delete_row_wid(this->get_id());
}

/*!
 * \brief Load the annotations for an assignment data object from a table
 * \param table The table
 * \param data The assignment data object
 * \return The vector of annotations
 */
std::vector<GAAnnotation*> GAAnnotation::load_from(DatabaseTable *table, GAAssignmentData *data) {
    std::vector<GAAnnotation*> found;
    sqlite3_stmt* statement = table->prepare_statement(table->prepare_select_all("assignment_data = " + DatabaseTable::escape_string(data->get_id())));
    while(sqlite3_step(statement) == SQLITE_ROW) {
        GAAnnotation* annot = new GAAnnotation(table->get_string(statement, 0), table->get_string(statement, 2));
        annot->set_assignment_data(data);
        annot->set_title(table->get_string(statement, 3));
        annot->set_description(table->get_string(statement, 4));
        annot->set_category(table->get_string(statement, 5));
        annot->set_points(DatabaseTable::get_int(statement, 6));
        annot->set_filename(DatabaseTable::get_string(statement, 7));
        annot->set_line(DatabaseTable::get_int(statement, 8));

        found.push_back(annot);
    }
    table->finalize_statement(statement);
    return found;
}
