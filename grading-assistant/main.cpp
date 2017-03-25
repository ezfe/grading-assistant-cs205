#include <iostream>

#include "gradingassistant.h"

using namespace std;

int main(int argc, char* argv[]) {

    GradingAssistant ga;

    Class* c = new Class();
    c->set_name("CS205 Section 1");

    Assignment* a = new Assignment();
    a->set_title("Assignment Title");
    a->set_description("Cool Description");

    c->add_assignment(a);

    ga.add_class(c);

    std::cout << ga.to_string() << std::endl;

    return 0;
}
