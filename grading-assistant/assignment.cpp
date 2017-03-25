#include "assignment.h"

Assignment::Assignment() {

}

std::string Assignment::get_title() {
    return this->title;
}

void Assignment::set_title(std::string title) {
    this->title = title;
}

std::string Assignment::get_description() {
    return this->description;
}

void Assignment::set_description(std::string description) {
    this->description = description;
}

std::string Assignment::to_string() {
    return "Assignment{" + this->get_title() + "}";
}
