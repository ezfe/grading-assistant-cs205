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

    std::vector<std::pair<std::string, std::string>> items = FileManager::get_files_in("/Users/ezekielelin/Desktop");
    for(std::pair<std::string, std::string> pair: items) {
        std::cout << pair.first << ": " << pair.second << std::endl;
    }

    ga->save();

    delete ga;

    database.close();
    settings.save();

    return 0;
}
