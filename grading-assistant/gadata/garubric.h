#ifndef GARUBRIC_H
#define GARUBRIC_H

#include <string>
#include <vector>

#include "gaidentifiableobject.h"
#include "garubricrow.h"

class GARubricRow;

class GARubric: public GAIdentifiableObject {
public:
    using GAIdentifiableObject::GAIdentifiableObject;
    GARubric(std::string title, int maxPoints);

    virtual ~GARubric();

    std::string get_title();
    void set_title(std::string t);

    std::vector<GARubricRow*> get_rows();
    GARubricRow* add_row(std::string category, std::string description, int pointValue);
    GARubricRow* add_row(std::string category, std::vector<std::string> descriptions, int pointValue);

    GARubricRow* get_ec();
    GARubricRow* set_ec(std::string category, std::string description, int pointValue);

    bool save_to(DatabaseTable* table);
private:
    std::string title;
    std::vector<GARubricRow*> rows;

    int maxPoints;
    GARubricRow* ec;
};

#endif // GARUBRIC_H
