#include "garubricrow.h"

GARubricRow::GARubricRow(std::string category, std::string description, int total_points): GAIdentifiableObject() {
    this->category = category;
    this->descriptions.push_back(description);
    this->points = total_points;
}

GARubricRow::GARubricRow(std::string category, std::vector<std::string> description, int total_points): GAIdentifiableObject() {
    this->category = category;
    this->descriptions = description;
    this->points = total_points;
}

GARubricRow::~GARubricRow() {
    /* This class currently doesn't own anything */
}

std::string GARubricRow::get_category() {
    if (this->is_extra_credit()) {
        return "Extra Credit";
    } else {
        return this->category;
    }
}

std::vector<std::string> GARubricRow::get_descriptions() {
    return descriptions;
}

void GARubricRow::add_description(std::string description) {
    this->descriptions.push_back(description);
}

void GARubricRow::set_category(std::string c) {
    if (this->is_extra_credit()) {
        this->category = "Extra Credit";
    } else {
        this->category = c;
    }
}

void GARubricRow::set_descriptions(std::vector<std::string> d) {
    descriptions = d;
}

void GARubricRow::set_max_points(int mP) {
    points = mP;
}

GARubric* GARubricRow::get_rubric() {
    return this->rubric;
}

/*!
 * \brief Set the rubric
 *
 * You should not call this, call add_row on a rubric
 *
 * \param rubric The rubric
 */
void GARubricRow::set_rubric(GARubric* rubric) {
    this->rubric = rubric;
}

bool GARubricRow::is_extra_credit() {
    return this->isExtraCredit;
}

void GARubricRow::set_extra_credit(bool i) {
    this->isExtraCredit = i;
}

int GARubricRow::get_max_points() {
    return points;
}

/*!
 * \brief Create an identical, but seperate rubric
 * \return The copied rubric
 */
GARubricRow* GARubricRow::copy() {
    if (this == nullptr) {
        return nullptr;
    }
    GARubricRow* newRow = new GARubricRow(this->category, this->descriptions, this->points);
    newRow->set_grading_assistant(this->get_grading_assistant());
    return newRow;
}

bool GARubricRow::save() {
    std::cout << "Starting save for GARubricRow " << this->get_category() << std::endl;

    if (this->get_grading_assistant() == nullptr) {
        std::cout << "- No grading assistant, not saving" << std::endl;
        return false;
    }

    DatabaseTable* rowTable = this->get_grading_assistant()->rubricRowTable;
    DatabaseTable* valuesTable = this->get_grading_assistant()->rubricRowValuesTable;

    std::string values = DatabaseTable::escape_string(this->get_id()) + ", ";
    values += DatabaseTable::escape_string(this->get_category()) + ", ";
    values += DatabaseTable::escape_string(std::to_string(this->points)) + ", ";
    values += DatabaseTable::escape_string(this->rubric->get_id()) + ", ";
    values += DatabaseTable::escape_string(std::to_string(this->isExtraCredit ? 1 : 0));
    rowTable->insert("id, category, total_points, rubric, extra_credit", values);

    int i = 0;
    for(std::string cell: this->get_descriptions()) {
        valuesTable->insert("id, value, rubric_row", std::to_string(i) + ", " + DatabaseTable::escape_string(cell) + ", " + DatabaseTable::escape_string(this->get_id()));
        i++;
    }

    return true;
}

/*!
 * \brief Remove this object from the table
 * \return Whether the delete was successful
 */
bool GARubricRow::remove() {
    GradingAssistant* ga = this->get_grading_assistant();

    //remove the row from the table
    bool first = ga->rubricRowTable->delete_row_wid(this->get_id());
    //remove the values from the table
    bool second = ga->rubricRowValuesTable->delete_row("rubric_row = " + DatabaseTable::escape_string(this->get_id()));

    return first && second;
}

std::vector<GARubricRow*> GARubricRow::load(GradingAssistant* ga, GARubric* rubric) {
    DatabaseTable* rubricRowTable = ga->rubricRowTable;
    DatabaseTable* rubricRowValuesTable = ga->rubricRowValuesTable;

    std::vector<GARubricRow*> found;
    std::string row_where = "rubric = " + DatabaseTable::escape_string(rubric->get_id());
    sqlite3_stmt* statement_row = rubricRowTable->prepare_statement(rubricRowTable->prepare_select_all(row_where));
    while(sqlite3_step(statement_row) == SQLITE_ROW) {
        GARubricRow* row = new GARubricRow(rubricRowTable->get_string(statement_row, 0));
        row->set_category(rubricRowTable->get_string(statement_row, 1));
        row->set_max_points(rubricRowTable->get_int(statement_row, 2));
        row->set_rubric(rubric);
        row->set_extra_credit(rubricRowTable->get_int(statement_row, 4) == 1 ? true : false);

        std::vector<std::string> found_descriptions;
        std::string desc_where = "rubric_row = " + DatabaseTable::escape_string(row->get_id());
        sqlite3_stmt* statement_descs = rubricRowValuesTable->prepare_statement(rubricRowValuesTable->prepare_select_all(desc_where));
        while (sqlite3_step(statement_descs) == SQLITE_ROW) {
            found_descriptions.push_back(rubricRowValuesTable->get_string(statement_descs, 1));
        }
        rubricRowValuesTable->finalize_statement(statement_descs);
        row->set_descriptions(found_descriptions);

        found.push_back(row);
    }
    rubricRowTable->finalize_statement(statement_row);

    return found;
}
