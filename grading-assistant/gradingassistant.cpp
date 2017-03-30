#include "gradingassistant.h"

GradingAssistant::GradingAssistant() {

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
