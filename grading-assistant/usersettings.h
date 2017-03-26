#ifndef USERSETTINGS_H
#define USERSETTINGS_H

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <ctime>

class UserSettings {
public:
    UserSettings(std::string path);
    UserSettings(UserSettings &obj);

    void operator =(UserSettings &obj);

    ~UserSettings();

    void save();
    void load();

    std::string get_path();
    void set_path(std::string path);

    void set(std::string key, std::string value);
    std::string getString(std::string key);

    void set(std::string key, int value);
    int getInt(std::string key);
private:
    UserSettings();

    std::map<std::string, std::string> stringValues;
    std::map<std::string, int> intValues;
    std::string path;

    const char COMMENT_MARK = '#';
    const std::string DELIMETER = " =: ";
    const char STRING_CHAR = 's';
    const char INTEGER_CHAR = 'i';
};

#endif // USERSETTINGS_H
