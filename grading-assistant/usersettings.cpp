#include "usersettings.h"

UserSettings::UserSettings() {
    std::cerr << "No path provided" << std::endl;
}

/*!
 * \brief Create a user settings object with a path
 * \param path
 */
UserSettings::UserSettings(std::string path) {
    this->path = path;
}

/*!
 * \brief Copy constructor
 * \param obj Object to copy
 */
UserSettings::UserSettings(UserSettings &obj) {
    this->path = obj.get_path();
}

/*!
 * \brief Assignment operator
 * \param obj Object to assign
 */
void UserSettings::operator =(UserSettings &obj) {
    this->path = obj.path;
}

UserSettings::~UserSettings() {

}

/*!
 * \brief Save all the data to the file
 */
void UserSettings::save() {
    std::ofstream fh;
    fh.open(this->path, std::ofstream::out | std::ofstream::trunc);

    fh << this->COMMENT_MARK << " Settings File" << std::endl;

    // Save the time
    time_t t = time(0);
    struct tm* now = localtime(&t);
    fh << this->COMMENT_MARK << " Saved on " <<  (now->tm_mon + 1) << "/" << now->tm_mday << "/" << (now->tm_year + 1900) << std::endl;

    // Save the delimeter
    fh << this->DELIMETER << std::endl;

    // Save the strings
    typedef std::map<std::string, std::string>::const_iterator StrIter;
    for(StrIter i = stringValues.begin(); i != stringValues.end(); ++i){
        fh << this->STRING_MARK << i->first << this->DELIMETER << i->second << std::endl;
    }

    // Save the numbers
    typedef std::map<std::string, int>::const_iterator IntIter;
    for(IntIter i = intValues.begin(); i != intValues.end(); ++i){
        fh << this->INTEGER_MARK << i->first << this->DELIMETER << i->second << std::endl;
    }

    fh.flush();
    fh.close();
}

/*!
 * \brief Load the settings from file
 */
void UserSettings::load() {
    // Clear the existing values
    this->stringValues.clear();
    this->intValues.clear();

    std::ifstream fh;
    std::string line;

    fh.open(this->path, std::ifstream::in);

    // Set the delimeter
    std::string delimeter = this->DELIMETER;
    int lineNumber = 0;
    while (std::getline(fh, line)) {
        if (line.at(0) == COMMENT_MARK) {
            continue;
        }
        if (lineNumber == 0) {
            delimeter = line;
        } else {
            int splitPosition = line.find(delimeter);
            char type = line.at(0);
            std::string key = line.substr(1, splitPosition - 1);
            std::string value = line.substr(splitPosition + delimeter.length());

            // Choose which variable to save to
            if (type == STRING_MARK) {
                stringValues[key] = value;
            } else if (type == INTEGER_MARK) {
                intValues[key] = std::stoi(value);
            } else {
                std::cerr << "Unexpected value type: " << type << std::endl;
            }
        }

        lineNumber++;
    }

    fh.close();
}

/*!
 * \brief Get the path
 * \return The path
 */
std::string UserSettings::get_path() {
    return this->path;
}

/*!
 * \brief Set the path
 * \param path The path
 */
void UserSettings::set_path(std::string path) {
    this->path = path;
}

/*!
 * \brief Get the string value of a key
 * \param key The key
 * \return The string value
 */
std::string UserSettings::getString(std::string key) {
    return this->stringValues[key];
}

/*!
 * \brief Get the integer value of a key
 * \param key The key
 * \return The integer value
 */
int UserSettings::getInt(std::string key) {
    return this->intValues[key];
}

/*!
 * \brief Set the key/value
 * \param key The key
 * \param value The value
 */
void UserSettings::set(std::string key, std::string value) {
    this->stringValues[key] = value;
}

/*!
 * \brief Set the key/value
 * \param key The key
 * \param value The value
 */
void UserSettings::set(std::string key, int value) {
    this->intValues[key] = value;
}
