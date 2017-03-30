#ifndef GRADINGASSISTANT_H
#define GRADINGASSISTANT_H

#include <vector>
#include "gadata/gaclass.h"

class GradingAssistant {
public:
    GradingAssistant();
    ~GradingAssistant();

    std::vector<GAClass*> get_classes();
    void add_class(GAClass* c);

    std::string to_string();

private:
    std::vector<GAClass*> classes;
};

#endif // GRADINGASSISTANT_H
