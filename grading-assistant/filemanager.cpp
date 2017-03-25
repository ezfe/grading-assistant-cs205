#include "filemanager.h"

FileManager::FileManager() {

}

std::string FileManager::get_data_directory() {
    const std::string app_name = "elin-sampsell.grading-assistant";
    if (__APPLE__) {
        return FileManager::expand_home("~/Library/Application Support/" + app_name + "/");
    } else {
        return "./";
    }
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
