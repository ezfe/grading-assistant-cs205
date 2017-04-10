#include "garubric.h"

GARubric::GARubric(std::string title) {
    this->title = title;
}

GARubric::~GARubric() {
    /* This class owns rubric rows */

    for(GARubricRow* row: this->rows) {
        delete row;
    }
    this->rows.clear();

    delete this->ec;
    this->ec = nullptr;
}

std::string GARubric::get_title() {
    return title;
}

void GARubric::set_title(std::string t) {
    title = t;
}

int GARubric::get_max_points() {
    int sum = 0;
    for(GARubricRow* row: this->get_rows()) {
        sum += row->get_max_points();
    }
    return sum;
}

std::vector<GARubricRow *> GARubric::get_rows() {
    return rows;
}

void GARubric::add_row(GARubricRow *row) {
    rows.push_back(row);
    row->set_rubric(this);
    row->set_extra_credit(false);
}

GARubricRow* GARubric::add_row(std::string category, std::string description, int pointValue) {
    GARubricRow* row = new GARubricRow(category, description, pointValue);
    this->add_row(row);
    return this->rows.back();
}

GARubricRow* GARubric::add_row(std::string category, std::vector<std::string> description, int pointValue) {
    GARubricRow* row = new GARubricRow(category, description, pointValue);
    this->add_row(row);
    return this->rows.back();
}

GARubricRow* GARubric::get_ec() {
    return this->ec;
}

GARubricRow* GARubric::set_ec(std::string c, std::string description, int pointValue) {
    this->set_ec(new GARubricRow(c, description, pointValue));
    return this->get_ec();
}

void GARubric::set_ec(GARubricRow* row) {
    if (this->ec != nullptr) {
        delete this->ec;
    }
    this->ec = row;
    row->set_rubric(this);
    row->set_extra_credit(true);
}

bool GARubric::save_to(DatabaseTable* table) {
    std::string values = DatabaseTable::escape_string(this->get_id()) + ", " + DatabaseTable::escape_string(this->title);
    return table->insert("id, title", values);
}

std::vector<GARubric*> GARubric::load_from(DatabaseTable* table) {
    std::vector<GARubric*> found;
    sqlite3_stmt* statement = table->prepare_statement(table->prepare_select_all());
    while (sqlite3_step(statement) == SQLITE_ROW) {
        GARubric* rubric = new GARubric(table->get_string(statement, 0));
        rubric->set_title(table->get_string(statement, 1));
        found.push_back(rubric);
    }
    table->finalize_statement(statement);
    return found;
}
