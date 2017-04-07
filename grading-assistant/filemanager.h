#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <string>
#include <iostream>
#include <stdlib.h>
#include "platform.h"
#include "gadata/gaclass.h"
#include "gadata/gaassignment.h"
#include "gadata/gastudent.h"
#include "gadata/gaassignmentdata.h"

#include <QDir>

class FileManager {
public:
    static std::string expand_home(std::string path);

    static void assure_directory_exists(std::string path);

    static std::string get_app_directory();
    static std::string get_settings_path();
    static std::string get_database_path();

    static std::string get_class_directory(GAClass* class_);
    static std::string get_assignment_directory(GAAssignment* assignment);
    static std::string get_assignment_student_directory(GAAssignment* assignment, GAStudent* student);
    static std::string get_assignment_student_directory(GAAssignmentData* assignment_data);

private:
    FileManager();
};

#endif // FILEMANAGER_H
