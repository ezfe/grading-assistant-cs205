#ifndef GAASSIGNMENT_H
#define GAASSIGNMENT_H

#include <iostream>
#include <string>

#include "gaidentifiableobject.h"
#include "../databasetable.h"
#include "gaclass.h"
#include "garubric.h"

class GAClass;
class GARubric;

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

    GARubric* get_rubric();
    void set_rubric(GARubric* rubric);

    bool save_to(DatabaseTable* table);
    static std::vector<GAAssignment*> load(GradingAssistant* ga, GAClass* class_);
private:
    std::string title;
    std::string description;

    GAClass* class_ = nullptr;
    GARubric* rubric = nullptr;
};

#endif // GAASSIGNMENT_H
