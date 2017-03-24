#include "annotation.h"

Annotation::Annotation() {

}

std::string Annotation::get_title() {
    return this->title;
}

void Annotation::set_title(std::string title) {
    this->title = title;
}

std::string Annotation::get_description() {
    return this->description;
}

void Annotation::set_description(std::string description) {
    this->description = description;
}

std::string Annotation::get_category() {
    return this->category;
}

void Annotation::set_category(std::string category) {
    this->category = category;
}

std::string Annotation::get_location() {
    return this->location;
}

void Annotation::set_location(std::string location) {
    this->location = location;
}
