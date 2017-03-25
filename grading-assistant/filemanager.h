#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <string>
#include <iostream>
#include <stdlib.h>

class FileManager {
public:
    FileManager();

    static std::string get_data_directory();
    static std::string expand_home(std::string path);
};

#endif // FILEMANAGER_H
