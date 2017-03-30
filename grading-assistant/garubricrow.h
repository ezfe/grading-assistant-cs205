#ifndef GARUBRICROW_H
#define GARUBRICROW_H

#include <string>
#include <vector>

#include "gaidentifiableobject.h"

class GARubricRow: public GAIdentifiableObject {
public:
    using GAIdentifiableObject::GAIdentifiableObject;

    GARubricRow(int id, std::string c, std::string d, int p);
    GARubricRow(int id, std::string c, std::vector<std::string> d, int p);

    ~GARubricRow();

    std::string get_category();
    std::vector<std::string> get_descriptions();

    int get_max_points();

    int get_earned_points();
    void set_earned_points(int p);

    virtual bool save_to(DatabaseTable* table);
private:
    std::string category;
    std::vector<std::string> descriptions;

    int maxPoints;
    int earnedPoints;
};

#endif // GARUBRICROW_H
