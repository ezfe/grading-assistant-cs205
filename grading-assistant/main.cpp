#include <iostream>

#include "gradingassistant.h"
#include "usersettings.h"
#include "filemanager.h"
#include "databasemanager.h"
#include "databasetable.h"
#include "gadata/garubric.h"
#include "gadata/garubricrow.h"
#include "gadata/gaassignment.h"

int main(int argc, char* argv[]) {

    /* === Set Up === */
    FileManager::assure_directory_exists(FileManager::get_app_directory());
    UserSettings settings(FileManager::get_settings_path());
    DatabaseManager database(FileManager::get_database_path());
    GradingAssistant* ga = new GradingAssistant();

    settings.load();
    database.open();

    /* === Data Creation === */

    GAClass* cs104 = new GAClass("CS 104");
    GAClass* cs205 = new GAClass("CS 205");

    GAStudent* ezekiel = new GAStudent("Ezekiel Elin", "eline");
    GAStudent* natalie = new GAStudent("Natalie Sampsell", "sampseln");
    GAStudent* student104a = new GAStudent("Student 104a", "stu104a");
    GAStudent* student104b = new GAStudent("Student 104b", "stu104b");
    GAStudent* student104c = new GAStudent("Student 104c", "stu104c");
    cs205->add_student(ezekiel);
    cs205->add_student(natalie);
    cs104->add_student(student104a);
    cs104->add_student(student104b);
    cs104->add_student(student104c);

    GAAssignment* assign1 = new GAAssignment();
    assign1->set_title("Lab 1");
    assign1->set_description("Super fun lab");
    cs104->add_assignment(assign1);

    GAAssignment* assign2 = new GAAssignment();
    assign2->set_title("Lab A");
    assign2->set_description("Super sad lab");

    cs205->add_assignment(assign2);

    GAAssignmentData* data1 = ezekiel->get_data(assign2);

//    GARubric* rubric = new GARubric("Test Rubric", 20);
//    rubric->save_to(rubricTable);

    ga->add_class(cs104);
    ga->add_class(cs205);

    /* === Clean Up === */
    ga->save(&database);
    delete ga;

    database.close();
    settings.save();

    return 0;
}
