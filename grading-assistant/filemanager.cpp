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
    QDir dir(QString::fromStdString(path));
    if (!dir.exists()) {
        dir.mkpath(".");
    }
}

std::string FileManager::get_app_directory() {
    const std::string app_name = "elin-sampsell.grading-assistant";
    std::string ret_val;
    if (GA_PLATFORM == GA_PLATFORM_APPLE) {
        ret_val = FileManager::expand_home("~/Library/Application Support//" + app_name + "/");
    } else if (GA_PLATFORM == GA_PLATFORM_LINUX) {
        ret_val = FileManager::expand_home("~/." + app_name + "/");
    } else {
        ret_val = "./" + app_name + "/";
    }
    return QDir::cleanPath(QString::fromStdString(ret_val)).toStdString();
}

std::string FileManager::get_settings_path() {
    return QDir::cleanPath(QString::fromStdString(FileManager::get_app_directory()) + QDir::separator() + "settings.txt").toStdString();
}

std::string FileManager::get_database_path() {
    return QDir::cleanPath(QString::fromStdString(FileManager::get_app_directory()) + QDir::separator() + "database.sqlite3").toStdString();
}
