#include "garubricrow.h"

GARubricRow::GARubricRow(int id, std::string c, std::string d, int p): GAIdentifiableObject(id) {
    category = c;
    descriptions.push_back(d);
    earnedPoints = p;
}

GARubricRow::GARubricRow(int id, std::string c, std::vector<std::string> d, int p): GAIdentifiableObject(id) {
    category = c;
    descriptions = d;
    maxPoints = p;
    earnedPoints = 0;
}

GARubricRow::~GARubricRow() {

}

std::string GARubricRow::get_category() {
    return category;
}

std::vector<std::string> GARubricRow::get_descriptions() {
    return descriptions;
}

int GARubricRow::get_max_points() {
    return maxPoints;
}

int GARubricRow::get_earned_points() {
    return earnedPoints;
}

void GARubricRow::set_earned_points(int p) {
    earnedPoints = p;
}

bool GARubricRow::save_to(DatabaseTable* table) {
    return table->insert("id, category", this->id_string() + ", " + this->category);
}