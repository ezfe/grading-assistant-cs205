#include "gradingassistant.h"

GradingAssistant::GradingAssistant() {
    this->start_id = 0;
}

GradingAssistant::GradingAssistant(int start_id) {
    this->start_id = start_id;
}

GradingAssistant::~GradingAssistant() {
    for(GAClass* aClass: this->classes) {
        delete aClass;
    }
}

std::vector<GAClass*> GradingAssistant::get_classes() {
    return this->classes;
}

void GradingAssistant::add_class(GAClass* c) {
    this->classes.push_back(c);
}

std::string GradingAssistant::to_string() {
    std::string temp = "GAssistant{";
    for(GAClass* c: this->classes) {
        temp += c->to_string() + ", ";
    }
    temp += "}";
    return temp;
}

int GradingAssistant::make_id() {
    return this->start_id++;
}

int GradingAssistant::get_start_id() {
    return this->start_id;
}

void GradingAssistant::set_start_id(int start_id) {
    this->start_id = start_id;
}
