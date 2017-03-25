#include "gradingassistant.h"

GradingAssistant::GradingAssistant() {

}

GradingAssistant::~GradingAssistant() {
    for(Class* aClass: this->classes) {
        delete aClass;
    }
}

std::vector<Class*> GradingAssistant::get_classes() {
    return this->classes;
}

void GradingAssistant::add_class(Class* c) {
    this->classes.push_back(c);
}

std::string GradingAssistant::to_string() {
    std::string temp = "GAssistant{";
    for(Class* c: this->classes) {
        temp += c->to_string() + ", ";
    }
    temp += "}";
    return temp;
}
