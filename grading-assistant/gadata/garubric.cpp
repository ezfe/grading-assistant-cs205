#include "garubric.h"

/*!
 * \brief Construct a GARubric with a title
 * \param title The title
 */
GARubric::GARubric(std::string title) {
    this->title = title;
}

/*!
 * \brief Deconstruct the GARubric
 *
 * This will desconstruct the rows of the rubric, including the extra credit row
 */
GARubric::~GARubric() {
    /* This class owns rubric rows */

    for(GARubricRow* row: this->rows) {
        delete row;
    }
    this->rows.clear();

    delete this->ec;
    this->ec = nullptr;
}

/*!
 * \brief Get the title
 * \return The title
 */
std::string GARubric::get_title() {
    return title;
}

/*!
 * \brief Set the title
 * \param t The title
 */
void GARubric::set_title(std::string t) {
    title = t;
}

/*!
 * \brief Get the max points (not including extra credit)
 *
 * This is currently calculated dyanmically based on all the rows
 *
 * This does not include extra credit, so it can be used to calculate %score.
 *
 * \return The number of points
 */
int GARubric::get_max_points() {
    int sum = 0;
    for(GARubricRow* row: this->get_rows()) {
        sum += row->get_max_points();
    }
    return sum;
}

/*!
 * \brief Get the list of rows
 * \return The list of rows
 */
std::vector<GARubricRow *> GARubric::get_rows() {
    return rows;
}

/*!
 * \brief Add a row
 * \param row The row
 */
void GARubric::add_row(GARubricRow *row) {
    rows.push_back(row);
    row->set_rubric(this);
    row->set_extra_credit(false);
}

/*!
 * \brief Create and add a row (with single description)
 * \param category The category
 * \param description The description (one single column)
 * \param pointValue The point value of the row
 * \return The row, already added
 */
GARubricRow* GARubric::add_row(std::string category, std::string description, int pointValue) {
    GARubricRow* row = new GARubricRow(category, description, pointValue);
    this->add_row(row);
    return this->rows.back();
}

/*!
 * \brief Create and add a row (with multiple descriptions)
 * \param category The category
 * \param description The description list
 * \param pointValue The point value of the row
 * \return The row, already added
 */
GARubricRow* GARubric::add_row(std::string category, std::vector<std::string> description, int pointValue) {
    GARubricRow* row = new GARubricRow(category, description, pointValue);
    this->add_row(row);
    return this->rows.back();
}

/*!
 * \brief Get the extra credit row
 * \return The extra credit row
 */
GARubricRow* GARubric::get_ec() {
    return this->ec;
}

/*!
 * \brief Create and set the extra credit row (single description)
 *
 * This will delete an existing extra credit object properly
 *
 * \param c The category
 * \param description The description (single column)
 * \param pointValue The point value of the row
 * \return The row, already set
 */
GARubricRow* GARubric::set_ec(std::string c, std::string description, int pointValue) {
    this->set_ec(new GARubricRow(c, description, pointValue));
    return this->get_ec();
}

/*!
 * \brief Set the extra credit row
 *
 * This will delete an existing extra credit object properly
 *
 * \param row The row
 */
void GARubric::set_ec(GARubricRow* row) {
    if (this->ec != nullptr) {
        delete this->ec;
    }
    this->ec = row;
    row->set_rubric(this);
    row->set_extra_credit(true);
}

/*!
 * \brief Save the rubric to a table
 *
 * Does not save constituent rows
 *
 * \param table The table
 * \return Whether the insert was successful
 */
bool GARubric::save_to(DatabaseTable* table) {
    std::string values = DatabaseTable::escape_string(this->get_id()) + ", " + DatabaseTable::escape_string(this->title);
    return table->insert("id, title", values);
}

/*!
 * \brief Load all the rubrics from a table
 * \param table The table
 * \return The list of rubrics
 */
std::vector<GARubric*> GARubric::load_from(DatabaseTable* table) {
    std::vector<GARubric*> found;
    sqlite3_stmt* statement = table->prepare_statement(table->prepare_select_all());
    while (sqlite3_step(statement) == SQLITE_ROW) {
        GARubric* rubric = new GARubric(table->get_string(statement, 0));
        rubric->set_title(table->get_string(statement, 1));
        found.push_back(rubric);
    }
    table->finalize_statement(statement);
    return found;
}
