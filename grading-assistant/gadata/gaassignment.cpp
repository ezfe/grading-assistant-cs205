#include "gaassignment.h"

std::string GAAssignment::get_title() {
    return this->title;
}

void GAAssignment::set_title(std::string title) {
    this->title = title;
}

std::string GAAssignment::get_description() {
    return this->description;
}

void GAAssignment::set_description(std::string description) {
    this->description = description;
}

std::string GAAssignment::to_string() {
    return "Assignment{" + this->get_title() + "}";
}

bool GAAssignment::save_to(DatabaseTable* table) {
    std::string values = DatabaseTable::escape_string(this->id_string()) + ", ";
    values += DatabaseTable::escape_string(this->title) + ", ";
    values += DatabaseTable::escape_string(this->description);
    return table->insert("id, title, description", values);
}
