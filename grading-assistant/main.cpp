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

    GAClass* c = ga->get_classes().front();
    GAStudent* s = c->get_students().front();

    std::cout << s->get_lafayette_username() << std::endl;
    s->set_lafayette_username("A");

    ga->save();

    delete ga;

    database.close();
    settings.save();

    return 0;
}
