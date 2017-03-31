#include "gaannotation.h"

GAAnnotation::GAAnnotation(std::string type): GAIdentifiableObject() {
    this->set_type(type);
}

GAAnnotation::GAAnnotation(std::string id, std::string type): GAIdentifiableObject(id) {
    this->set_type(type);
}

GAAnnotation::GAAnnotation(std::string id, std::string type, std::string title): GAIdentifiableObject(id) {
    this->set_type(type);
    this->title = title;
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

void GAAnnotation::set_type(std::string type) {
    if (type == GA_ANNOTATION_COMMENT || type == GA_ANNOTATION_EXTRACREDIT || type == GA_ANNOTATION_PROBLEM) {
        this->category = type;
    } else {
        this->category = GA_ANNOTATION_UNSET;
    }
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
    if (this->data == nullptr) {
        return false;
    }
    std::string values = DatabaseTable::escape_string(this->get_id()) + ", ";
    values += DatabaseTable::escape_string(this->data->get_id()) + ", ";
    values += DatabaseTable::escape_string(this->type) + ", ";
    values += DatabaseTable::escape_string(this->title) + ", ";
    values += DatabaseTable::escape_string(this->description) + ", ";
    values += DatabaseTable::escape_string(this->category) + ", ";
    values += DatabaseTable::escape_string(this->location);
    return table->insert("id, assignment_data, type, title, description, category, location", values);
}

std::vector<GAAnnotation*> GAAnnotation::load_from(DatabaseTable *table, GAAssignmentData *data) {
    std::vector<GAAnnotation*> found;
    sqlite3_stmt* statement = table->prepare_statement(table->prepare_select_all("assignment_data = " + DatabaseTable::escape_string(data->get_id())));
    while(sqlite3_step(statement) == SQLITE_ROW) {
        GAAnnotation* annot = new GAAnnotation(table->get_string(statement, 0), table->get_string(statement, 2));
        annot->set_assignment_data(data);
        annot->set_title(table->get_string(statement, 3));
        annot->set_description(table->get_string(statement, 4));
        annot->set_category(table->get_string(statement, 5));
        annot->set_location(table->get_string(statement, 6));

        found.push_back(annot);
    }
    table->finalize_statement(statement);
    return found;
}

std::string GAAnnotation::to_string() {
    return "Annotation{" + this->get_title() + "@" + this->get_location() + "}";
}
