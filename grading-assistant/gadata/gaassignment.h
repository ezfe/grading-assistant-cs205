#ifndef GAASSIGNMENT_H
#define GAASSIGNMENT_H

#include <iostream>
#include <string>

#include "gaidentifiableobject.h"
#include "gaclass.h"
#include "garubric.h"
#include "../gradingassistant.h"

class GAClass;
class GradingAssistant;

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

    void set_rubric_promise(std::string p);
    void resolve_promise();

    bool save_to(DatabaseTable* table);
    static std::vector<GAAssignment*> load_from(DatabaseTable* table, GAClass* class_);
private:
    std::string title;
    std::string description;

    GAClass* class_ = nullptr;
    GARubric* rubric = nullptr;

    /*!
     * \brief Used to store ID of future rubric pointer during loading process
     */
    std::string __rubric_promise = "";
};

#endif // GAASSIGNMENT_H
