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
    int get_max_points();
    void set_max_points(int mP);

    std::vector<GARubricRow*> get_rows();
    void add_row(GARubricRow* row);
    GARubricRow* add_row(std::string category, std::string description, int pointValue);
    GARubricRow* add_row(std::string category, std::vector<std::string> descriptions, int pointValue);

    GARubricRow* get_ec();
    GARubricRow* set_ec(std::string category, std::string description, int pointValue);
    void set_ec(GARubricRow* row);

    bool save_to(DatabaseTable* table);
    static std::vector<GARubric*> load_from(DatabaseTable* table);
private:
    std::string title;
    std::vector<GARubricRow*> rows;

    int maxPoints;
    GARubricRow* ec = nullptr;
};

#endif // GARUBRIC_H
