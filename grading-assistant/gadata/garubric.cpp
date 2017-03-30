#include "garubric.h"

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
    rows.push_back(new GARubricRow(category, descriptions, pointValue));
}

void GARubric::set_ec(std::string c, std::string description, int pointValue) {
    if (ec != nullptr) {
        delete ec;
    }
    ec = new GARubricRow(c, description, pointValue);
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
    return table->insert("id, title", values);
}
