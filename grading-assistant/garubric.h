#ifndef GARUBRIC_H
#define GARUBRIC_H

#include <string>
#include <vector>

#include "garubricrow.h"

class GARubric {
public:
    GARubric();
    ~GARubric();

    std::string get_title();
    void set_title(std::string t);

    std::vector<GARubricRow *> get_rows();

    void add_row(std::string category, std::vector<std::string> descriptions, int pointValue);
    void set_ec(std::string category, std::string description, int pointValue);

    double calculate_score();

private:
    std::string title;
    std::vector<GARubricRow *> rows;

    int maxPoints;
    GARubricRow * ec;
};

#endif // GARUBRIC_H
