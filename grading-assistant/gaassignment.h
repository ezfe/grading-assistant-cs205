#ifndef GAASSIGNMENT_H
#define GAASSIGNMENT_H

#include <iostream>
#include <string>

#include "gaidentifiableobject.h"

class GAAssignment: public GAIdentifiableObject {
public:
    using GAIdentifiableObject::GAIdentifiableObject;

    std::string get_title();
    void set_title(std::string title);

    std::string get_description();
    void set_description(std::string description);

    virtual bool save_to(DatabaseTable* table);

    std::string to_string();
private:
    std::string title;
    std::string description;
};

#endif // GAASSIGNMENT_H
