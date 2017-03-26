#include "gastudent.h"

GAStudent::~GAStudent() {

}

std::string GAStudent::get_name() {
    return this->name;
}

void GAStudent::set_name(std::string name) {
    this->name = name;
}

std::string GAStudent::get_lafayette_username() {
    return this->lafayette_username;
}

void GAStudent::set_lafayette_username(std::string username) {
    this->lafayette_username = username;
}

std::string GAStudent::to_string() {
    return "Student{" + this->get_name() + "}";
}
