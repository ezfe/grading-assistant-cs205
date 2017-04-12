#include <iostream>

#include "gradingassistant.h"
#include "usersettings.h"
#include "filemanager.h"
#include "databasemanager.h"
#include "databasetable.h"
#include "gadata/garubric.h"
#include "gadata/garubricrow.h"
#include "gadata/gaassignment.h"
#include "gadata/gaannotation.h"

int main(int argc, char* argv[]) {

    /* === Set Up === */
    FileManager::assure_directory_exists(FileManager::get_app_directory());
    UserSettings settings(FileManager::get_settings_path());
    DatabaseManager database(FileManager::get_database_path());
    GradingAssistant* ga = new GradingAssistant(&database);

    settings.load();
    database.open();

    ga->load();

    GAClass* cl = ga->get_class("{d27912bb-f5f5-48dd-8b10-858add23694d}");
    GAStudent* student = cl->get_students().back();
    GAAssignment* assign = cl->get_assignments().back();
    GAAssignmentData* data = student->get_data(assign);
    std::cout << data->get_student()->get_name() << std::endl;
    std::cout << data->get_assignment()->get_title() << std::endl;

    FileManager::assure_directory_exists(FileManager::get_assignment_student_directory(data));

    std::vector<std::string> files = data->query_files();
    for(std::string path: files) {
        std::cout << path << std::endl;
    }

    delete ga;

    database.close();
    settings.save();

    return 0;
}
