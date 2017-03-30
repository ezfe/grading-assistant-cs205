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

void GAStudent::add_data(GAAssignment * a, GAAssignmentData * d) {
    this->assignmentData.insert(std::pair<GAAssignment*, GAAssignmentData*>(a, d));
}

GAAssignmentData* GAStudent::get_data(GAAssignment *a) {
    return this->assignmentData.at(a);
}

std::map<GAAssignment*, GAAssignmentData*> GAStudent::get_map() {
    return this->assignmentData;
}

bool GAStudent::save_to(DatabaseTable* table) {
    std::string escaped_id = DatabaseTable::escape_string(this->id_string());
    std::string escaped_name = DatabaseTable::escape_string(this->name);
    std::string escaped_laf = DatabaseTable::escape_string(this->lafayette_username);
    return table->insert("id, name, lafayette_username", escaped_id + ", " + escaped_name + ", " + escaped_laf);
}

std::string GAStudent::to_string() {
    return "Student{" + this->get_name() + "}";
}
