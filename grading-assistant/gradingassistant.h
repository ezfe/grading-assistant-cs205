#ifndef GRADINGASSISTANT_H
#define GRADINGASSISTANT_H

#include <vector>
#include "class.h"

class GradingAssistant {
public:
    GradingAssistant();
    ~GradingAssistant();

    std::vector<Class*> get_classes();
    void add_class(Class* c);

    std::string to_string();
private:
    std::vector<Class*> classes;
};

#endif // GRADINGASSISTANT_H
