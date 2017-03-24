#ifndef STUDENT_H
#define STUDENT_H

#include <iostream>
#include <string>

class Student {
public:
    Student();
    ~Student();

    std::string get_name();
    void set_name(std::string name);

    std::string get_lafayette_username();
    void set_lafayette_username(std::string username);
private:
    std::string name;
    std::string lafayette_username;
};

#endif // STUDENT_H
