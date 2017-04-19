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
}

/*!
 * \brief Get the location
 * \return The location
 */
std::string GAAnnotation::get_location() {
    return this->location;
}

/*!
 * \brief Set the location
 * \param location The location
 */
void GAAnnotation::set_location(std::string location) {
    this->location = location;
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
}

/*!
 * \brief Save the annotation to a table
 * \param table The table
 * \return Whether the insert was successful
 */
bool GAAnnotation::save_to(DatabaseTable* table) {
    if (this->data == nullptr) {
        return false;
    }
    std::string values = DatabaseTable::escape_string(this->get_id()) + ", ";
    values += DatabaseTable::escape_string(this->data->get_id()) + ", ";
    values += DatabaseTable::escape_string(this->type) + ", ";
    values += DatabaseTable::escape_string(this->title) + ", ";
    values += DatabaseTable::escape_string(this->description) + ", ";
    values += DatabaseTable::escape_string(this->category) + ", ";
    values += DatabaseTable::escape_string(this->location);
    return table->insert("id, assignment_data, type, title, description, category, location", values);
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
        annot->set_location(table->get_string(statement, 6));

        found.push_back(annot);
    }
    table->finalize_statement(statement);
    return found;
}
