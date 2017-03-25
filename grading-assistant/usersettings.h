#ifndef USERSETTINGS_H
#define USERSETTINGS_H

#include <iostream>
#include <fstream>
#include <string>
#include <map>

class UserSettings {
public:
    UserSettings();
    UserSettings(std::string path);
    UserSettings(UserSettings &obj);

    void operator =(UserSettings &obj);

    ~UserSettings();

    void save();
    void load();

    std::string get_path();
    void set_path(std::string path);

private:
    std::map<std::string, std::string> values;
    std::string path;

    const std::string delimeter = " =: ";
};

#endif // USERSETTINGS_H
