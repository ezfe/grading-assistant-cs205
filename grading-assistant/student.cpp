#include "student.h"

Student::Student() {

}

Student::~Student() {

}

std::string Student::get_name() {
    return this->name;
}

void Student::set_name(std::string name) {
    this->name = name;
}

std::string Student::get_lafayette_username() {
    return this->lafayette_username;
}

void Student::set_lafayette_username(std::string username) {
    this->lafayette_username = username;
}

std::string Student::to_string() {
    return "Student{" + this->get_name() + "}";
}
