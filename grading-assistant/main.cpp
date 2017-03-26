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

    /* Program Loop */

    while (true) {
        std::cout << "Commands:" << std::endl;
        std::cout << "  Create a class" << std::endl;
        std::cout << "    0" << std::endl;
        int iter_main = 1;
        if (ga.get_classes().size() > 0) {
            std::cout << "  Select a class" << std::endl;
            for(Class* c: ga.get_classes()) {
                std::cout << "    " << iter_main++ << " (" << c->get_name() << ")" << std::endl;
            }
        }
        std::cout << "  Exit program" << std::endl;
        std::cout << "    " << iter_main << std::endl;

        int action;
        std::cin >> action;
        std::cin.ignore(1, '\n');

        if (action <= 0) {
            Class* c = new Class();
            ga.add_class(c);
            std::cout << "Enter class name: ";

            std::string name;
            std::getline(std::cin, name);
            c->set_name(name);

            std::cout << "Created class" << std::endl;
        } else if (action < iter_main) {
            std::cout << "Selecting class..." << std::endl;
            Class* c = ga.get_classes()[action - 1];

            std::cout << c->get_name() << std::endl;

            while (true) {
                std::cout << "Commands:" << std::endl;
                std::cout << "  Create..." << std::endl;
                std::cout << "    0 (student)" << std::endl;
                std::cout << "    1 (assignment)" << std::endl;
                int iter_class = 2;
                if (c->get_students().size() > 0) {
                    std::cout << "  Select a student" << std::endl;
                    for(Student* student: c->get_students()) {
                        std::cout << "    " << iter_class++ << " (" << student->get_name() << ")" << std::endl;
                    }
                }

                int start_assignments = iter_class;
                if (c->get_assignments().size() > 0) {
                    std::cout << "  Select an assignment" << std::endl;
                    for(Assignment* assignment: c->get_assignments()) {
                        std::cout << "    " << iter_class++ << " (" << assignment->get_title() << ")" << std::endl;
                    }
                }
                std::cout << "  Exit class" << std::endl;
                std::cout << "    " << iter_class << std::endl;

                int cl_action;
                std::cin >> cl_action;
                std::cin.ignore(1, '\n');

                if (cl_action <= 0) {
                    Student* student = new Student();
                    c->add_student(student);

                    std::cout << "Enter name: ";
                    std::string name;
                    std::getline(std::cin, name);
                    student->set_name(name);

                    std::cout << "Enter Lafayette username: ";
                    std::string lafid;
                    std::getline(std::cin, lafid);
                    student->set_lafayette_username(lafid);

                    std::cout << "Created student" << std::endl;
                } else if (cl_action == 1) {
                    Assignment* assignment = new Assignment();
                    c->add_assignment(assignment);

                    std::cout << "Enter title: ";
                    std::string title;
                    std::getline(std::cin, title);
                    assignment->set_title(title);

                    std::cout << "Enter description (1 line): ";
                    std::string desc;
                    std::getline(std::cin, desc);
                    assignment->set_description(desc);

                    std::cout << "Created assignment" << std::endl;
                } else if (cl_action < start_assignments) {
                    std::cout << "Selecting student..." << std::endl;
                    Student* student = c->get_students()[cl_action - 2];

                    std::cout << student->get_name() << std::endl;
                    std::cout << student->get_lafayette_username() << std::endl;
                } else if (cl_action < iter_class) {
                    std::cout << "Selecting assignment..." << std::endl;
                    Assignment* assignment = c->get_assignments()[cl_action - start_assignments];

                    std::cout << assignment->get_title() << std::endl;
                    std::cout << assignment->get_description() << std::endl;
                } else if (cl_action >= iter_class) {
                    std::cout << "Exiting " << c->get_name() << "..." << std::endl;
                    break;
                }
            }
        } else if (action >= iter_main) {
            std::cout << "Exiting..." << std::endl;
            break;
        }
    }

    /* Close up program */

    database.close();
    settings.save();

    return 0;
}
