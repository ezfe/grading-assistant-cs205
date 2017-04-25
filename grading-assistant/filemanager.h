#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <string>
#include <iostream>
#include <stdlib.h>
#include <vector>

#include <QDir>
#include <QFile>
#include <QDirIterator>

#include "platform.h"
#include "gadata/gaclass.h"
#include "gadata/gaassignment.h"
#include "gadata/gastudent.h"
#include "gadata/gaassignmentdata.h"
#include "gradingassistant.h"

class GARubric;
class GAStudent;
class GAClass;
class GAAssignment;

class FileManager {
public:
    static std::string get_home();
    static std::string expand_home(std::string path);

    static void assure_directory_exists(std::string path);

    static std::string get_app_directory_location();

    static std::string get_app_name();
    static std::string get_app_directory();
    static std::string get_settings_path();
    static std::string get_database_path();

    static std::string get_class_directory(GAClass* class_);
    static std::string get_assignment_directory(GAAssignment* assignment);
    static std::string get_assignment_student_directory(GAAssignment* assignment, GAStudent* student);
    static std::string get_assignment_student_directory(GAAssignmentData* assignment_data);

    static std::string append(std::string path, std::string appending);
    static std::string append(std::string path, std::string appending, std::string appending_2);

    static std::vector<std::string> import(std::string path, GradingAssistant* ga, GAAssignment* assign);

    static std::vector<std::pair<std::string, std::string>> get_files_in(std::string path);
    static void copy_directory(std::string p1, std::string p2);
private:
    FileManager();
};

#endif // FILEMANAGER_H
