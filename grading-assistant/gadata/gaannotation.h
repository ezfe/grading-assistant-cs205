#ifndef GAANNOTATION_H
#define GAANNOTATION_H

#define GA_ANNOTATION_COMMENT "GA_ANNOTATION_COMMENT"
#define GA_ANNOTATION_EXTRACREDIT "GA_ANNOTATION_EC"
#define GA_ANNOTATION_PROBLEM "GA_ANNOTATION_PROBLEM"
#define GA_ANNOTATION_UNSET ""

#include <iostream>
#include <string>

#include "gaidentifiableobject.h"
#include "gaassignmentdata.h"
#include "../databasetable.h"

class GAAssignmentData;

class GAAnnotation: public GAIdentifiableObject {
public:
    using GAIdentifiableObject::GAIdentifiableObject;
    GAAnnotation(std::string type);
    GAAnnotation(std::string id, std::string type);
    GAAnnotation(std::string id, std::string type, std::string title);

    virtual ~GAAnnotation();

    std::string get_type();
    void set_type(std::string type);

    std::string get_title();
    void set_title(std::string title);

    std::string get_description();
    void set_description(std::string description);

    std::string get_category();
    void set_category(std::string category);

    std::string get_filename();
    void set_filename(std::string filename);

    int get_line();
    void set_line(int line);

    GAAssignmentData* get_assignment_data();
    void set_assignment_data(GAAssignmentData* data);

    int get_points();
    void set_points(int value);

    bool save_to(DatabaseTable* table);
    static std::vector<GAAnnotation*> load_from(DatabaseTable* table, GAAssignmentData* data);
private:
    std::string type;
    std::string title;
    std::string description;
    std::string category;
    std::string filename;
    int line;

    int value = 0;

    GAAssignmentData* data = nullptr;
};

#endif // GAANNOTATION_H
