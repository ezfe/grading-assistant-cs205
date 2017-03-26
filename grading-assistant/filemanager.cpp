#include "filemanager.h"

FileManager::FileManager() {

}

std::string FileManager::expand_home(std::string path) {
    if (path.at(0) == '~' && path.at(1) == '/') {
        char* temp = getenv("HOME");
        if (temp != NULL) {
            std::string home_path = std::string(temp);
            return home_path + path.substr(1);
        } else {
            std::cerr << "Unable to get HOME" << std::endl;
        }
    } else {
        return path;
    }
}

void FileManager::assure_directory_exists(std::string path) {
    std::string temp = "mkdir \"" + path + "\"";
    system(temp.c_str());
}

std::string FileManager::get_app_directory() {
    const std::string app_name = "elin-sampsell.grading-assistant";
    if (GA_PLATFORM == GA_PLATFORM_APPLE) {
        return FileManager::expand_home("~/Library/Application Support/" + app_name + "/");
    } else if (GA_PLATFORM == GA_PLATFORM_LINUX) {
        return FileManager::expand_home("~/." + app_name + "/");
    } else {
        return "./" + app_name + "/";
    }
}

std::string FileManager::get_settings_path() {
    return FileManager::get_app_directory() + "settings.txt";
}

std::string FileManager::get_database_path() {
    return FileManager::get_app_directory() + "database.sqlite3";
}
