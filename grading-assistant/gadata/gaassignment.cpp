#include "gaassignment.h"

GAAssignment::~GAAssignment() {
    /* This class currently owns nothing */
}

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

GAClass* GAAssignment::get_class() {
    return this->class_;
}

void GAAssignment::set_class(GAClass* class_) {
    this->class_ = class_;
}

std::string GAAssignment::to_string() {
    return "Assignment{" + this->get_title() + "}";
}

bool GAAssignment::save_to(DatabaseTable* table) {
    if (this->class_ == nullptr) {
        //Don't save assignments not attached to a class
        return false;
    }

    std::string values = DatabaseTable::escape_string(this->id_string()) + ", ";
    values += DatabaseTable::escape_string(this->title) + ", ";
    values += DatabaseTable::escape_string(this->description) + ", ";
    values += DatabaseTable::escape_string(this->class_->id_string());

    return table->insert("id, title, description, class", values);
}
