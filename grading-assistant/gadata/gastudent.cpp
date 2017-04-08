#include "gastudent.h"

GAStudent::GAStudent(std::string name, std::string laf_id): GAIdentifiableObject() {
    this->name = name;
    this->lafayette_username = laf_id;
}

GAStudent::GAStudent(std::string id, std::string name, std::string laf_id): GAIdentifiableObject(id) {
    this->name = name;
    this->lafayette_username = laf_id;
}

GAStudent::~GAStudent() {
    /* This class currently owns GAAssignmentData */

    for (auto const& x: this->assignmentData) {
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

void GAStudent::set_data(GAAssignment* a, GAAssignmentData* d) {
    delete this->assignmentData[a];
    this->assignmentData[a] = d;
    d->set_assignment(a);
    d->set_student(this);
}

GAAssignmentData* GAStudent::get_data(GAAssignment* a) {
    if (a->get_class() != this->get_class()) {
        std::cerr << "This student has no data for that assignment" << std::endl;
        return nullptr;
    }
    if (this->assignmentData[a] == nullptr) {
        this->set_data(a, new GAAssignmentData());
    }
    return this->assignmentData[a];
}

std::map<GAAssignment*, GAAssignmentData*> GAStudent::get_map() {
    return this->assignmentData;
}

bool GAStudent::save_to(DatabaseTable* table) {
    std::string escaped_id = DatabaseTable::escape_string(this->get_id());
    std::string escaped_name = DatabaseTable::escape_string(this->name);
    std::string escaped_laf = DatabaseTable::escape_string(this->lafayette_username);
    if (this->class_ != nullptr) {
        std::string escaped_class_id = DatabaseTable::escape_string(this->class_->get_id());
        return table->insert("id, name, lafayette_username, class", escaped_id + ", " + escaped_name + ", " + escaped_laf + ", " + escaped_class_id);
    } else {
        return table->insert("id, name, lafayette_username", escaped_id + ", " + escaped_name + ", " + escaped_laf);
    }
}

void GAStudent::remove_from(DatabaseTable* table) {
    table->single_exec("DELETE FROM " + table->get_name() + " WHERE id = " + DatabaseTable::escape_string(this->get_id()));
}

std::vector<GAStudent*> GAStudent::load_from(DatabaseTable* table, GAClass* class_) {
    std::vector<GAStudent*> found;
    sqlite3_stmt* statement = table->prepare_statement(table->prepare_select_all("class = " + DatabaseTable::escape_string(class_->get_id())));
    while(sqlite3_step(statement) == SQLITE_ROW) {
        GAStudent* s = new GAStudent(table->get_string(statement, 0), table->get_string(statement, 1), table->get_string(statement, 2));
        found.push_back(s);
    }
    table->finalize_statement(statement);
    return found;
}

std::string GAStudent::to_string() {
    return "Student{" + this->get_name() + "}";
}
