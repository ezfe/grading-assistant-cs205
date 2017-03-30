#include "garubric.h"

GARubric::~GARubric() {
    for(GARubricRow* row: this->rows) {
        delete row;
    }
    delete ec;
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

void GARubric::add_row(int id, std::string category, std::vector<std::string> descriptions,
                       int pointValue) {
    rows.push_back(new GARubricRow(id, category, descriptions, pointValue));
}

void GARubric::set_ec(int id, std::string c, std::string description, int pointValue) {
    if (ec != nullptr) {
        delete ec;
    }
    ec = new GARubricRow(id, c, description, pointValue);
}

double GARubric::calculate_score() {
    double total = 0;
    for(GARubricRow* row : this->rows) {
        total += row->get_earned_points();
    }
    return (total + ec->get_earned_points())/maxPoints;
}

bool GARubric::save_to(DatabaseTable* table) {
    return table->insert("id, title", this->id_string() + ", " + this->title);
}
