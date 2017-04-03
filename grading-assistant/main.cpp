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

    /* === Data Creation === */

//    ga->load();


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

    GAAssignment* cs104_assign1 = new GAAssignment();
    cs104_assign1->set_title("104 Assign 1");
    cs104_assign1->set_description("104 Assign 1 Description");
    cs104->add_assignment(cs104_assign1);

    GAAssignment* cs104_assign2 = new GAAssignment();
    cs104_assign2->set_title("104 Assign 2");
    cs104_assign2->set_description("104 Assign 2 Description");
    cs104->add_assignment(cs104_assign2);

    GAAssignment* cs205_assign1 = new GAAssignment();
    cs205_assign1->set_title("205 Assign 1");
    cs205_assign1->set_description("205 Assign 1 Description");
    cs205->add_assignment(cs205_assign1);

    GAAssignment* cs205_assign2 = new GAAssignment();
    cs205_assign2->set_title("205 Assign 2");
    cs205_assign2->set_description("205 Assign 2 Description");
    cs205->add_assignment(cs205_assign2);

    GAAssignmentData* data1 = ezekiel->get_data(cs205_assign1);
    GAAssignmentData* data2 = ezekiel->get_data(cs205_assign2);

    GAAssignmentData* data3 = student104a->get_data(cs104_assign1);
    GAAssignmentData* data4 = student104a->get_data(cs104_assign2);

    GAAnnotation* annot = new GAAnnotation(GA_ANNOTATION_COMMENT);
    data1->add_annotation(annot);

    GAAnnotation* annot2 = new GAAnnotation(GA_ANNOTATION_PROBLEM);
    data2->add_annotation(annot2);

    GARubric* simpleRubric = new GARubric("Simple Rubric", 20);
    simpleRubric->add_row("Correctness", "Sample Correct Desc.", 5);
    simpleRubric->add_row("Style", "Sample Style Desc.", 5);
    simpleRubric->add_row("Awesomeness", "Sample Awe. Desc.", 10);
    simpleRubric->set_ec("Extra Cred!", "LOL", 2);

    GARubric* complexRubric = new GARubric("Complex Rubric", 20);
    GARubricRow* cxcorr = complexRubric->add_row("CX Correctness", "CX CORRECT A", 5);
    cxcorr->add_description("CX COR B");
    cxcorr->add_description("CX COR C");
    GARubricRow* cxsty = complexRubric->add_row("CX Style", "CX STYLE A", 5);
    cxsty->add_description("CX STY B");
    cxsty->add_description("CX STY C");
    GARubricRow* cxawe = complexRubric->add_row("CX Awesomeness", "CX AWE A", 10);
    cxawe->add_description("CX AWE B");
    cxawe->add_description("CX AWE C");
    GARubricRow* complexEC = complexRubric->set_ec("CX Extra Cred!", "CX EC A", 2);
    complexEC->add_description("CX EC B");
    complexEC->add_description("CX EC C");

    ga->add_rubric(simpleRubric);
    ga->add_rubric(complexRubric);

    ga->add_class(cs104);
    ga->add_class(cs205);

    /* === Clean Up === */
    ga->save();
    delete ga;

    database.close();
    settings.save();

    return 0;
}
