#include "garubricrow.h"

GARubricRow::GARubricRow(std::string c, std::string d, int p): GAIdentifiableObject() {
    category = c;
    descriptions.push_back(d);
    points = p;
}

GARubricRow::GARubricRow(std::string c, std::vector<std::string> d, int p): GAIdentifiableObject() {
    category = c;
    descriptions = d;
    points = p;
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
    values += DatabaseTable::escape_string(this->rubric->get_id());
    rowTable->insert("id, category, rubric", values);

    int i = 0;
    for(std::string cell: this->get_descriptions()) {
        valuesTable->insert("id, value, rubric_row", std::to_string(i) + ", " + DatabaseTable::escape_string(cell) + ", " + DatabaseTable::escape_string(this->get_id()));
        i++;
    }

    return true;
}
