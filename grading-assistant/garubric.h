#ifndef GARUBRIC_H
#define GARUBRIC_H

#include <string>
#include <vector>

#include "gaidentifiableobject.h"
#include "garubricrow.h"

class GARubric: public GAIdentifiableObject {
public:
    using GAIdentifiableObject::GAIdentifiableObject;
    ~GARubric();

    std::string get_title();
    void set_title(std::string t);

    std::vector<GARubricRow *> get_rows();

    void add_row(int id, std::string category, std::vector<std::string> descriptions, int pointValue);
    void set_ec(int id, std::string category, std::string description, int pointValue);

    double calculate_score();

    virtual bool save_to(DatabaseTable* table);
private:
    std::string title;
    std::vector<GARubricRow *> rows;

    int maxPoints;
    GARubricRow * ec;
};

#endif // GARUBRIC_H
