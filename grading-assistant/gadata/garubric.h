#ifndef GARUBRIC_H
#define GARUBRIC_H

#include <string>
#include <vector>

#include "gaidentifiableobject.h"
#include "../databasetable.h"
#include "garubricrow.h"
#include "../gradingassistant.h"

class GARubricRow;

class GARubric: public GAIdentifiableObject {
public:
    using GAIdentifiableObject::GAIdentifiableObject;
    GARubric(std::string title);

    virtual ~GARubric();

    std::string get_title();
    void set_title(std::string t);
    int get_max_points();

    std::vector<GARubricRow*> get_rows();
    void add_row(GARubricRow* row);
    GARubricRow* add_row(std::string category, std::string description, int pointValue);
    GARubricRow* add_row(std::string category, std::vector<std::string> descriptions, int pointValue);

    GARubricRow* get_ec();
    GARubricRow* set_ec(std::string category, std::string description, int pointValue);
    void set_ec(GARubricRow* row);

    GARubric* copy();

    void save();
    static std::vector<GARubric*> load(GradingAssistant* ga);
    static GARubric* load(GradingAssistant* ga, std::string id);
    static GARubric* extract_single(sqlite3_stmt* statement, GradingAssistant *ga);
private:
    std::string title;
    std::vector<GARubricRow*> rows;

    GARubricRow* ec = nullptr;
};

#endif // GARUBRIC_H
