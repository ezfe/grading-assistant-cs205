#include "usersettings.h"

UserSettings::UserSettings() {
    std::cerr << "No path provided" << std::endl;
}

UserSettings::UserSettings(std::string path) {
    this->path = path;
}

UserSettings::UserSettings(UserSettings &obj) {
    this->path = obj.get_path();
}

void UserSettings::operator =(UserSettings &obj) {
    this->path = obj.path;
}

UserSettings::~UserSettings() {

}

void UserSettings::save() {
    std::ofstream fh;
    fh.open(this->path, std::ofstream::out | std::ofstream::trunc);

    fh << this->COMMENT_MARK << " Settings File" << std::endl;

    time_t t = time(0);
    struct tm* now = localtime(&t);
    fh << this->COMMENT_MARK << " Saved on " <<  (now->tm_mon + 1) << "/" << now->tm_mday << "/" << (now->tm_year + 1900) << std::endl;

    fh << this->DELIMETER << std::endl;

    typedef std::map<std::string, std::string>::const_iterator StrIter;
    for(StrIter i = stringValues.begin(); i != stringValues.end(); ++i){
        fh << this->STRING_MARK << i->first << this->DELIMETER << i->second << std::endl;
    }

    typedef std::map<std::string, int>::const_iterator IntIter;
    for(IntIter i = intValues.begin(); i != intValues.end(); ++i){
        fh << this->INTEGER_MARK << i->first << this->DELIMETER << i->second << std::endl;
    }

    fh.flush();
    fh.close();
}

void UserSettings::load() {
    this->stringValues.clear();
    this->intValues.clear();

    std::ifstream fh;
    std::string line;

    fh.open(this->path, std::ifstream::in);

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

std::string UserSettings::get_path() {
    return this->path;
}

void UserSettings::set_path(std::string path) {
    this->path = path;
}

std::string UserSettings::getString(std::string key) {
    return this->stringValues[key];
}

int UserSettings::getInt(std::string key) {
    return this->intValues[key];
}

void UserSettings::set(std::string key, std::string value) {
    this->stringValues[key] = value;
}

void UserSettings::set(std::string key, int value) {
    this->intValues[key] = value;
}

bool UserSettings::existsString(std::string key) {
    auto it = this->stringValues.find(key);
    return it != this->stringValues.end();
}

bool UserSettings::existsInt(std::string key) {
    auto it = this->intValues.find(key);
    return it != this->intValues.end();
}
