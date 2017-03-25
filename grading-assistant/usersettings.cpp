#include "usersettings.h"

UserSettings::UserSettings() {
    std::cerr << "UserSettings::UserSettings() No path provided" << std::endl;
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

    fh << this->delimeter << std::endl;

    typedef std::map<std::string, std::string>::const_iterator Iter;
    for(Iter i = values.begin(); i != values.end(); ++i){
        fh << i->first << this->delimeter << i->second << std::endl;
    }

    fh.flush();
    fh.close();
}

void UserSettings::load() {
    this->values.clear();

    std::ifstream fh;
    std::string line;

    fh.open(this->path, std::ifstream::in);

    std::string delimeter = NULL;
    while (std::getline(fh, line)) {
        if (delimeter == NULL) {
            delimeter = line;
        } else {
            int splitPosition = line.find(delimeter);
            std::string key = line.substr(0, splitPosition);
            std::string value = line.substr(splitPosition + delimeter.length());
            values[key] = value;
        }
    }

    fh.close();
}

std::string UserSettings::get_path() {
    return this->path;
}

void UserSettings::set_path(std::string path) {
    this->path = path;
}
