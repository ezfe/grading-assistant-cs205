#include <iostream>

#include "gradingassistant.h"
#include "usersettings.h"
#include "filemanager.h"

int main(int argc, char* argv[]) {

    FileManager::assure_directory_exists(FileManager::get_data_directory());
    GradingAssistant ga;
    UserSettings settings(FileManager::get_settings_path());
    settings.load();



    settings.save();

    return 0;
}
