#ifndef GRADINGASSISTANT_H
#define GRADINGASSISTANT_H

#include <vector>
#include "gaclass.h"

class GradingAssistant {
public:
    GradingAssistant();
    GradingAssistant(int start_id);
    ~GradingAssistant();

    std::vector<GAClass*> get_classes();
    void add_class(GAClass* c);

    std::string to_string();

    int make_id();
    int get_start_id();
    void set_start_id(int start_id);
private:
    std::vector<GAClass*> classes;

    int start_id = 0;
};

#endif // GRADINGASSISTANT_H
