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
    return category;
}

std::vector<std::string> GARubricRow::get_descriptions() {
    return descriptions;
}

void GARubricRow::add_description(std::string description) {
    this->descriptions.push_back(description);
}

void GARubricRow::set_category(std::string c) {
    category = c;
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

void GARubricRow::set_rubric(GARubric* rubric) {
    this->rubric = rubric;
}


int GARubricRow::get_max_points() {
    return points;
}

bool GARubricRow::save_to(DatabaseTable* rowTable, DatabaseTable* valuesTable) {
    std::string values = DatabaseTable::escape_string(this->get_id()) + ", ";
    values += DatabaseTable::escape_string(this->category) + ", ";
    values += DatabaseTable::escape_string(std::to_string(this->points)) + ", ";
    values += DatabaseTable::escape_string(this->rubric->get_id());
    rowTable->insert("id, category, total_points, rubric", values);

    int i = 0;
    for(std::string cell: this->get_descriptions()) {
        valuesTable->insert("id, value, rubric_row", std::to_string(i) + ", " + DatabaseTable::escape_string(cell) + ", " + DatabaseTable::escape_string(this->get_id()));
        i++;
    }

    return true;
}

GARubricRow* GARubricRow::load_from(DatabaseTable* rubricRowTable, DatabaseTable* rubricRowValuesTable, GARubric* rubric) {
    GARubricRow* found = nullptr;
    std::string row_where = "rubric = " + DatabaseTable::escape_string(rubric->get_id());
    sqlite3_stmt* statement_row = rubricRowTable->prepare_statement(rubricRowTable->prepare_select_all(row_where));
    if (sqlite3_step(statement_row) == SQLITE_ROW) {
        found = new GARubricRow(rubricRowTable->get_string(statement_row, 0));
        found->set_category(rubricRowTable->get_string(statement_row, 1));
        found->set_max_points(rubricRowTable->get_int(statement_row, 2));
        found->set_rubric(rubric);
    } else {
        return nullptr;
    }
    rubricRowTable->finalize_statement(statement_row);

    std::vector<std::string> found_descriptions;
    std::string desc_where = "rubric_row = " + DatabaseTable::escape_string(found->get_id());
    sqlite3_stmt* statement_descs = rubricRowValuesTable->prepare_statement(rubricRowValuesTable->prepare_select_all(desc_where));
    while (sqlite3_step(statement_descs) == SQLITE_ROW) {
        found_descriptions.push_back(rubricRowValuesTable->get_string(statement_descs, 1));
    }
    rubricRowValuesTable->finalize_statement(statement_descs);
    found->set_descriptions(found_descriptions);

    return found;
}
