#ifndef CLASS_H
#define CLASS_H

#include <iostream>
#include <string>
#include <vector>

#include <student.h>

class Class {
public:
    Class();
    ~Class();

private:
    std::string name;
    std::vector<Student> students;
};

#endif // CLASS_H
