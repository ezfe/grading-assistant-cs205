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

    std::vector<std::string>  res = ga->query_annotation("erat nulla tempus");
    for(std::string r: res) {
        std::cout << r << std::endl;
    }

    std::cout << FileManager::append(FileManager::get_app_directory(), ".git") << std::endl;

    delete ga;

    database.close();
    settings.save();

    return 0;
}
