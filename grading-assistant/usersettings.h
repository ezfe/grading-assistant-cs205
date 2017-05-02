#ifndef USERSETTINGS_H
#define USERSETTINGS_H

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <ctime>

/*!
 * \brief The settings manager for the program
 */
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

    /// The map of string values
    std::map<std::string, std::string> stringValues;
    /// The map of integer values
    std::map<std::string, int> intValues;

    /// The path
    std::string path;

    /// The marking used to indicate a comment line
    const char COMMENT_MARK = '#';
    /// The marking used to indicate a string
    const char STRING_MARK = 's';
    /// The marking used to indicate an integer
    const char INTEGER_MARK = 'i';

    /// The marking used as a delimeter
    const std::string DELIMETER = " =: ";
};

#endif // USERSETTINGS_H
