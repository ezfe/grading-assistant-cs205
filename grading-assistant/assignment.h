#ifndef ASSIGNMENT_H
#define ASSIGNMENT_H

#include <iostream>
#include <string>

class Assignment {
public:
    Assignment();

    std::string get_title();
    void set_title(std::string title);

    std::string get_description();
    void set_description(std::string description);
private:
    std::string title;
    std::string description;
};

#endif // ASSIGNMENT_H
