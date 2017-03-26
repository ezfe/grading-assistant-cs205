#include <iostream>

#include "gradingassistant.h"
#include "usersettings.h"
#include "filemanager.h"
#include "databasemanager.h"

int main(int argc, char* argv[]) {

    FileManager::assure_directory_exists(FileManager::get_app_directory());
    GradingAssistant ga;
    UserSettings settings(FileManager::get_settings_path());
    DatabaseManager database(FileManager::get_database_path());

    settings.load();
    database.open();

//    std::cout << database.db() << std::endl;

    database.close();
    settings.save();

    return 0;
}
