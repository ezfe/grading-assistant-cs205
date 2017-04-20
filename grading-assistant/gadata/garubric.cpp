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
    row->set_grading_assistant(this->get_grading_assistant());
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
    row->set_grading_assistant(this->get_grading_assistant());
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
    row->set_grading_assistant(this->get_grading_assistant());
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
    row->set_grading_assistant(this->get_grading_assistant());
}

/*!
 * \brief Create a copy of the rubric
 * \return The copy of the rubric
 */
GARubric* GARubric::copy() {
    GARubric* newRubric = new GARubric(this->title + " (copy)");
    newRubric->set_ec(this->get_ec()->copy());
    for(GARubricRow* row: this->get_rows()) {
        newRubric->add_row(row->copy());
    }
    return newRubric;
}

/*!
 * \brief Save the rubric to a table
 *
 * \param table The table
 * \return Whether the insert was successful
 */
void GARubric::save() {
    DatabaseTable* table = this->get_grading_assistant()->rubricTable;

    std::string values = DatabaseTable::escape_string(this->get_id()) + ", " + DatabaseTable::escape_string(this->title);
    table->insert("id, title", values);

    for(GARubricRow* row: this->get_rows()) {
        std::cout << "  Saved rubric row " << row->get_category() << std::endl;

        // Save the row
        row->save();
    }

    // Check for extra credit
    if (this->get_ec() != nullptr) {
        // Save the extra credit
        this->get_ec()->save();
    }

}

/*!
 * \brief Load all the rubrics
 * \param rubricTable The rubric table
 * \param rubricRowTable The rubric row table
 * \param rubricRowValuesTable The rubric row values table
 * \return The list of rubrics
 */
std::vector<GARubric*> GARubric::load(GradingAssistant* ga) {
    DatabaseTable* rubricTable = ga->rubricTable;

    std::vector<GARubric*> found;
    sqlite3_stmt* statement = rubricTable->prepare_statement(rubricTable->prepare_select_all());
    while (sqlite3_step(statement) == SQLITE_ROW) {
        found.push_back(GARubric::extract_single(statement, ga));
    }
    rubricTable->finalize_statement(statement);
    return found;
}

/*!
 * \brief Load a single rubric by persistence ID
 * \param rubricTable The rubric table
 * \param rubricRowTable The rubric row table
 * \param rubricRowValuesTable The rubric row values table
 * \param id The persistence ID
 * \return The rubric
 */
GARubric* GARubric::load(GradingAssistant* ga, std::string id) {
    DatabaseTable* rubricTable = ga->rubricTable;

    GARubric* rubric = nullptr;
    sqlite3_stmt* statement = rubricTable->prepare_statement(rubricTable->prepare_select_all("id = " + DatabaseTable::escape_string(id)));
    if (sqlite3_step(statement) == SQLITE_ROW) {
        rubric = GARubric::extract_single(statement, ga);
    }
    DatabaseTable::finalize_statement(statement);
    return rubric;

}

/*!
 * \brief Extract a single rubric from a sqlite statement
 * \param statement The statement
 * \param rubricRowTable The rubric row table
 * \param rubricRowValuesTable The rubric row values table
 * \return The rubric
 */
GARubric* GARubric::extract_single(sqlite3_stmt* statement, GradingAssistant* ga) {
    DatabaseTable* rubricRowTable = ga->rubricRowTable;
    DatabaseTable* rubricRowValuesTable = ga->rubricRowValuesTable;

    GARubric* rubric = new GARubric(DatabaseTable::get_string(statement, 1));
    rubric->set_grading_assistant(ga);
    rubric->set_id(DatabaseTable::get_string(statement, 0));

    std::vector<GARubricRow*> rows = GARubricRow::load(ga, rubric);
    for(GARubricRow* row: rows) {
        if (row->is_extra_credit()) {
            rubric->set_ec(row);
        } else {
            rubric->add_row(row);
        }
    }

    return rubric;
}
