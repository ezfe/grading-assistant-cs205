#ifndef USERSETTINGS_H
#define USERSETTINGS_H

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <ctime>

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

    void set(std::string key, std::string value);
    std::string get(std::string key);

private:
    std::map<std::string, std::string> values;
    std::string path;

    const char COMMENT_MARK = '#';
    const std::string DELIMETER = " =: ";
};

#endif // USERSETTINGS_H
