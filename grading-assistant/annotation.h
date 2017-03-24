#ifndef ANNOTATION_H
#define ANNOTATION_H

#include <iostream>
#include <string>

class Annotation {
public:
    Annotation();

    std::string get_title();
    void set_title(std::string title);

    std::string get_description();
    void set_description(std::string description);

    std::string get_category();
    void set_category(std::string category);

    std::string get_location();
    void set_location(std::string location);
private:
    std::string title;
    std::string description;
    std::string category;
    std::string location;
};

#endif // ANNOTATION_H
