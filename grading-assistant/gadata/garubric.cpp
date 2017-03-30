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

std::vector<GARubricRow *> GARubric::get_rows() {
    return rows;
}

void GARubric::add_row(std::string category, std::vector<std::string> descriptions,
                       int pointValue) {
    GARubricRow* row = new GARubricRow(category, descriptions, pointValue);
    rows.push_back(row);
    row->set_rubric(this);
}

GARubricRow* GARubric::get_ec() {
    return this->ec;
}

void GARubric::set_ec(std::string c, std::string description, int pointValue) {
    if (ec != nullptr) {
        delete ec;
    }
    ec = new GARubricRow(c, description, pointValue);
    ec->set_rubric(this);
}

double GARubric::calculate_score() {
    double total = 0;
    for(GARubricRow* row : this->rows) {
        total += row->get_earned_points();
    }
    return (total + ec->get_earned_points())/maxPoints;
}

bool GARubric::save_to(DatabaseTable* table) {
    std::string values = DatabaseTable::escape_string(this->id_string()) + ", " + DatabaseTable::escape_string(this->title);
    values += ", " + std::to_string(this->maxPoints);
    return table->insert("id, title, max_points", values);
}
