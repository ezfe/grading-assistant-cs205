#include <iostream>

#include "gradingassistant.h"
#include "usersettings.h"
#include "filemanager.h"
#include "databasemanager.h"

int main(int argc, char* argv[]) {

    /* === Set Up === */
    FileManager::assure_directory_exists(FileManager::get_app_directory());
    UserSettings settings(FileManager::get_settings_path());
    DatabaseManager database(FileManager::get_database_path());
    GradingAssistant* ga = new GradingAssistant(&database);

    settings.load();
    database.open();

    ga->load();

//    GAAssignment* assign = ga->get_classes().front()->get_assignments().front();
//    FileManager::import("/Users/ezekielelin/Desktop/ImportDir", ga, assign);

    ga->save();

    delete ga;

    database.close();
    settings.save();

    return 0;
}
