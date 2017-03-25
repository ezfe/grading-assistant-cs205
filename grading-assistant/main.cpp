#include <iostream>

#include "gradingassistant.h"
#include "usersettings.h"
#include "filemanager.h"

using namespace std;

int main(int argc, char* argv[]) {

    GradingAssistant ga;

    Class* c = new Class();
    c->set_name("CS205 Section 1");

    Assignment* a = new Assignment();
    a->set_title("Assignment Title");
    a->set_description("Cool Description");

    c->add_assignment(a);

    ga.add_class(c);

    std::cout << ga.to_string() << std::endl;


    FileManager::assure_directory_exists(FileManager::get_data_directory());

    /* User Settings */
    UserSettings settings(FileManager::get_settings_path());
    settings.load();

    std::cout << settings.get("test") << std::endl;
    settings.set("test","yay");
    std::cout << settings.get("test") << std::endl;

    settings.save();

    return 0;
}
