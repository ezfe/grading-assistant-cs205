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

    typedef std::map<std::string, std::string>::const_iterator Iter;
    for(Iter i = values.begin(); i != values.end(); ++i){
        fh << i->first << this->DELIMETER << i->second << std::endl;
    }

    fh.flush();
    fh.close();
}

void UserSettings::load() {
    this->values.clear();

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
            std::string key = line.substr(0, splitPosition);
            std::string value = line.substr(splitPosition + delimeter.length());
            values[key] = value;
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

std::string UserSettings::get(std::string key) {
    return this->values[key];
}

void UserSettings::set(std::string key, std::string value) {
    this->values[key] = value;
}
