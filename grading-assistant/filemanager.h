#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <string>
#include <iostream>
#include <stdlib.h>
#include "platform.h"

#include <QDir>

class FileManager {
public:
    static std::string expand_home(std::string path);

    static void assure_directory_exists(std::string path);

    static std::string get_app_directory();
    static std::string get_settings_path();
    static std::string get_database_path();

private:
    FileManager();
};

#endif // FILEMANAGER_H
