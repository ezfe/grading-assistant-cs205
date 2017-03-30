#ifndef GAANNOTATION_H
#define GAANNOTATION_H

#include <iostream>
#include <string>

#include "gaidentifiableobject.h"
#include "gaassignmentdata.h"

class GAAssignmentData;

class GAAnnotation: public GAIdentifiableObject {
public:
    using GAIdentifiableObject::GAIdentifiableObject;
    GAAnnotation(std::string type);

    virtual ~GAAnnotation();

    std::string get_type();

    std::string get_title();
    void set_title(std::string title);

    std::string get_description();
    void set_description(std::string description);

    std::string get_category();
    void set_category(std::string category);

    std::string get_location();
    void set_location(std::string location);

    GAAssignmentData* get_assignment_data();
    void set_assignment_data(GAAssignmentData* data);

    virtual bool save_to(DatabaseTable* table);

    std::string to_string();
private:
    const std::string type;
    std::string title;
    std::string description;
    std::string category;
    std::string location;
    GAAssignmentData* data = nullptr;
};

#endif // GAANNOTATION_H
