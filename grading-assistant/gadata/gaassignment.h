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

/*!
 * \brief An assignment
 */
class GAAssignment: public GAIdentifiableObject {
public:
    using GAIdentifiableObject::GAIdentifiableObject;
    virtual ~GAAssignment();

    std::string get_title();
    void set_title(std::string title, bool save = true);

    std::string get_description();
    void set_description(std::string description, bool save = true);

    GAClass* get_class();
    void set_class(GAClass* class_, bool save = true);

    GARubric* get_rubric();
    void set_rubric(GARubric* rubric, bool save = true);

    void save(bool cascade);
    virtual bool remove();
    static std::vector<GAAssignment*> load(GradingAssistant* ga, GAClass* class_);
private:
    std::string title;
    std::string description;

    GAClass* class_ = nullptr;
    GARubric* rubric = nullptr;
};

#endif // GAASSIGNMENT_H
