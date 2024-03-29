#ifndef GAIDENTIFIABLEOBJECT_H
#define GAIDENTIFIABLEOBJECT_H

#include <iostream>
#include <string>
#include <sqlite3.h>
#include <QUuid>

class GradingAssistant;

/*!
 * \brief The base class for objects in the system
 *
 * Provides a persistent identifier
 */
class GAIdentifiableObject {
public:
    GAIdentifiableObject();
    GAIdentifiableObject(std::string id);
    GAIdentifiableObject(GradingAssistant* ga);
    GAIdentifiableObject(std::string id, GradingAssistant* ga);
    virtual ~GAIdentifiableObject();

    virtual bool remove() = 0;

    std::string get_id();
    void set_id(std::string id);

    GradingAssistant* get_grading_assistant();
    void set_grading_assistant(GradingAssistant* ga);
private:
    std::string id = "{00000000-0000-0000-0000-000000000000}";

    GradingAssistant* grading_assistant = nullptr;
};

#endif // GAIDENTIFIABLEOBJECT_H
