#include <iostream>

#include "gradingassistant.h"
#include "usersettings.h"
#include "filemanager.h"
#include "databasemanager.h"
#include "databasetable.h"

int main(int argc, char* argv[]) {

    /* === Set Up === */
    FileManager::assure_directory_exists(FileManager::get_app_directory());
    UserSettings settings(FileManager::get_settings_path());
    DatabaseManager database(FileManager::get_database_path());
    GradingAssistant ga;

    settings.load();
    database.open();


    std::cout << "Data Directory:" << std::endl << FileManager::get_app_directory() << std::endl;

    //demo

    //make class and table
    DatabaseTable* classesTable = new DatabaseTable(&database, "Classes", "id TEXT, name TEXT");
    DatabaseTable* studentTable = new DatabaseTable(&database, "Students", "id TEXT, name TEXT, lafayette_username TEXT, class TEXT");
    classesTable->drop();
    classesTable->create();

    studentTable->drop();
    studentTable->create();

    GAClass* cs104 = new GAClass("CS 104");
    cs104->save_to(classesTable);

    GAClass* cs205 = new GAClass("CS 205");
    cs205->save_to(classesTable);

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

    ezekiel->save_to(studentTable);
    natalie->save_to(studentTable);
    student104a->save_to(studentTable);
    student104b->save_to(studentTable);
    student104c->save_to(studentTable);

    /* === Clean Up === */
    database.close();
    settings.save();

    return 0;
}
