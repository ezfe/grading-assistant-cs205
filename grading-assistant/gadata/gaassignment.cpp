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

    std::string values = DatabaseTable::escape_string(this->get_id()) + ", ";
    values += DatabaseTable::escape_string(this->title) + ", ";
    values += DatabaseTable::escape_string(this->description) + ", ";
    values += DatabaseTable::escape_string(this->class_->get_id());

    return table->insert("id, title, description, class", values);
}

std::vector<GAAssignment*> GAAssignment::load_from(DatabaseTable* table, GAClass* class_) {
    std::vector<GAAssignment*> found;
    sqlite3_stmt* statement = table->prepare_statement(table->prepare_select_all("class = " + DatabaseTable::escape_string(class_->get_id())));
    while(sqlite3_step(statement) == SQLITE_ROW) {
        GAAssignment* assignment = new GAAssignment(table->get_string(statement, 0));
        assignment->set_title(table->get_string(statement, 1));
        assignment->set_description(table->get_string(statement, 2));
        assignment->set_class(class_);

        found.push_back(assignment);
    }
    table->finalize_statement(statement);
    return found;
}
