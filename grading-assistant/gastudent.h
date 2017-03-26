#ifndef GASTUDENT_H
#define GASTUDENT_H

#include <iostream>
#include <string>

#include "gaidentifiableobject.h"

class GAStudent: public GAIdentifiableObject {
public:
    using GAIdentifiableObject::GAIdentifiableObject;

    ~GAStudent();

    std::string get_name();
    void set_name(std::string name);

    std::string get_lafayette_username();
    void set_lafayette_username(std::string username);

    std::string to_string();
private:
    std::string name;
    std::string lafayette_username;
};

#endif // GASTUDENT_H
