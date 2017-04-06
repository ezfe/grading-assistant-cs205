#ifndef GARUBRICROW_H
#define GARUBRICROW_H

#include <string>
#include <vector>

#include "gaidentifiableobject.h"
#include "garubric.h"

class GARubric;

class GARubricRow: public GAIdentifiableObject {
public:
    using GAIdentifiableObject::GAIdentifiableObject;

    GARubricRow(std::string category, std::string description, int total_points);
    GARubricRow(std::string category, std::vector<std::string> description, int total_points);

    virtual ~GARubricRow();

    std::string get_category();
    std::vector<std::string> get_descriptions();
    void add_description(std::string description);

    void set_category(std::string c);
    void set_descriptions(std::vector<std::string> d);
    void set_max_points(int mP);

    GARubric* get_rubric();
    void set_rubric(GARubric* rubric);

    int get_max_points();

    bool save_to(DatabaseTable* rowTable, DatabaseTable *valuesTable);
    static std::vector<GARubricRow*> load_from(DatabaseTable* rubricRowTable, DatabaseTable *rubricRowValuesTable, GARubric* rubric);
private:
    std::string category;
    std::vector<std::string> descriptions;

    GARubric* rubric = nullptr;

    int points;
};

#endif // GARUBRICROW_H
