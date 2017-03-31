#ifndef GAASSIGNMENT_H
#define GAASSIGNMENT_H

#include <iostream>
#include <string>

#include "gaidentifiableobject.h"
#include "gaclass.h"

class GAClass;

class GAAssignment: public GAIdentifiableObject {
public:
    using GAIdentifiableObject::GAIdentifiableObject;
    virtual ~GAAssignment();

    std::string get_title();
    void set_title(std::string title);

    std::string get_description();
    void set_description(std::string description);

    GAClass* get_class();
    void set_class(GAClass* class_);

    virtual bool save_to(DatabaseTable* table);
    static std::vector<GAAssignment*> load_from(DatabaseTable* table, GAClass* class_);

    std::string to_string();
private:
    std::string title;
    std::string description;

    GAClass* class_ = nullptr;
};

#endif // GAASSIGNMENT_H
