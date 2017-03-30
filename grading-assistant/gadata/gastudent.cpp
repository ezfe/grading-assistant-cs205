#include "gastudent.h"

GAStudent::GAStudent(std::string name, std::string laf_id): GAIdentifiableObject() {
    this->name = name;
    this->lafayette_username = laf_id;
}

GAStudent::~GAStudent() {
    /* This class currently owns GAAssignmentData */
    std::cout << "~GAStudent()" << std::endl;

    for (auto const& x: this->assignmentData) {
        std::cout << x.first  << ':' << x.second << std::endl ;
        delete x.second;
    }
    this->assignmentData.clear();
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

GAClass* GAStudent::get_class() {
    return this->class_;
}

void GAStudent::set_class(GAClass* class_) {
    this->class_ = class_;
}

void GAStudent::set_data(GAAssignment * a, GAAssignmentData * d) {
    delete this->assignmentData[a];
    this->assignmentData[a] = d;
}

GAAssignmentData* GAStudent::get_data(GAAssignment* a) {
    return this->assignmentData[a];
}

std::map<GAAssignment*, GAAssignmentData*> GAStudent::get_map() {
    return this->assignmentData;
}

bool GAStudent::save_to(DatabaseTable* table) {
    std::string escaped_id = DatabaseTable::escape_string(this->id_string());
    std::string escaped_name = DatabaseTable::escape_string(this->name);
    std::string escaped_laf = DatabaseTable::escape_string(this->lafayette_username);
    if (this->class_ != nullptr) {
        std::string escaped_class_id = DatabaseTable::escape_string(this->class_->id_string());
        return table->insert("id, name, lafayette_username, class", escaped_id + ", " + escaped_name + ", " + escaped_laf + ", " + escaped_class_id);
    } else {
        return table->insert("id, name, lafayette_username", escaped_id + ", " + escaped_name + ", " + escaped_laf);
    }
}

std::string GAStudent::to_string() {
    return "Student{" + this->get_name() + "}";
}
