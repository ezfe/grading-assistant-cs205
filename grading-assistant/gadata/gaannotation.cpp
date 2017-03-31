#include "gaannotation.h"

GAAnnotation::GAAnnotation(std::string type): GAIdentifiableObject() {
    this->category = type;
}

GAAnnotation::~GAAnnotation() {
    /* This class currently owns nothing */
}

std::string GAAnnotation::get_title() {
    return this->title;
}

std::string GAAnnotation::get_type() {
    return this->type;
}

void GAAnnotation::set_title(std::string title) {
    this->title = title;
}

std::string GAAnnotation::get_description() {
    return this->description;
}

void GAAnnotation::set_description(std::string description) {
    this->description = description;
}

std::string GAAnnotation::get_category() {
    return this->category;
}

void GAAnnotation::set_category(std::string category) {
    this->category = category;
}

std::string GAAnnotation::get_location() {
    return this->location;
}

void GAAnnotation::set_location(std::string location) {
    this->location = location;
}

GAAssignmentData* GAAnnotation::get_assignment_data() {
    return this->data;
}

void GAAnnotation::set_assignment_data(GAAssignmentData *data) {
    this->data = data;
}

bool GAAnnotation::save_to(DatabaseTable* table) {
    std::string values = this->get_id() + ", " + this->type + ", " + this->title + ", " + this->description + ", " + this->category + ", " + this->location;
    return table->insert("id, type, title, description, category, location", values);
}

std::string GAAnnotation::to_string() {
    return "Annotation{" + this->get_title() + "@" + this->get_location() + "}";
}
