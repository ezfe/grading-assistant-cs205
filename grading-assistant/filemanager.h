#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <string>
#include <iostream>
#include <stdlib.h>
#include "platform.h"

class FileManager {
public:
    static std::string expand_home(std::string path);

    static void assure_directory_exists(std::string path);

    static std::string get_data_directory();
    static std::string get_settings_path();

private:
    FileManager();
};

#endif // FILEMANAGER_H
