#include "garubric.h"

GARubric::GARubric(std::string title, int maxPoints) {
    this->title = title;
    this->maxPoints = maxPoints;
}

GARubric::~GARubric() {
    /* This class owns rubric rows */

    for(GARubricRow* row: this->rows) {
        delete row;
    }
    this->rows.clear();

    delete this->ec;
    this->ec = nullptr;
}

std::string GARubric::get_title() {
    return title;
}

void GARubric::set_title(std::string t) {
    title = t;
}

int GARubric::get_max_points() {
    return maxPoints;
}

void GARubric::set_max_points(int mP) {
    maxPoints = mP;
}

std::vector<GARubricRow *> GARubric::get_rows() {
    return rows;
}

GARubricRow* GARubric::add_row(std::string category, std::string description, int pointValue) {
    GARubricRow* row = new GARubricRow(category, description, pointValue);
    rows.push_back(row);
    row->set_rubric(this);
    return row;
}

GARubricRow* GARubric::add_row(std::string category, std::vector<std::string> description, int pointValue) {
    GARubricRow* row = new GARubricRow(category, description, pointValue);
    rows.push_back(row);
    row->set_rubric(this);
    return row;
}

GARubricRow* GARubric::get_ec() {
    return this->ec;
}

GARubricRow* GARubric::set_ec(std::string c, std::string description, int pointValue) {
    if (ec != nullptr) {
        delete ec;
    }
    ec = new GARubricRow(c, description, pointValue);
    ec->set_rubric(this);
    return this->get_ec();
}

bool GARubric::save_to(DatabaseTable* table) {
    std::string values = DatabaseTable::escape_string(this->get_id()) + ", " + DatabaseTable::escape_string(this->title);
    values += ", " + std::to_string(this->maxPoints);
    return table->insert("id, title, max_points", values);
}
