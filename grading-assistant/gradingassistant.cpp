#include "gradingassistant.h"

GradingAssistant::GradingAssistant() {

}

GradingAssistant::~GradingAssistant() {
    for(GAClass* aClass: this->classes) {
        delete aClass;
    }
}

std::vector<GAClass*> GradingAssistant::get_classes() {
    return this->classes;
}

void GradingAssistant::add_class(GAClass* c) {
    this->classes.push_back(c);
}

std::string GradingAssistant::to_string() {
    std::string temp = "GAssistant{";
    for(GAClass* c: this->classes) {
        temp += c->to_string() + ", ";
    }
    temp += "}";
    return temp;
}

bool GradingAssistant::save(DatabaseManager* database) {

    DatabaseTable* annotationTable = new DatabaseTable(database, "Annotations", "id TEXT, type TEXT, title TEXT, description TEXT, category TEXT, location TEXT");
    annotationTable->drop();
    annotationTable->create();

    DatabaseTable* assignmentTable = new DatabaseTable(database, "Assignments", "id TEXT, title TEXT, description TEXT, class TEXT");
    assignmentTable->drop();
    assignmentTable->create();

    //TODO
//    DatabaseTable* assignmentDataTable = new DatabaseTable(database, "AssignmentData", "id TEXT, title TEXT, description TEXT");
//    assignmentDataTable->drop();
//    assignmentDataTable->create();

    DatabaseTable* classesTable = new DatabaseTable(database, "Classes", "id TEXT, name TEXT");
    classesTable->drop();
    classesTable->create();

    DatabaseTable* rubricTable = new DatabaseTable(database, "Rubrics", "id TEXT, title TEXT, max_points INTEGER");
    rubricTable->drop();
    rubricTable->create();

    DatabaseTable* rubricRowTable = new DatabaseTable(database, "RubricRows", "id TEXT, category TEXT, rubric TEXT");
    rubricRowTable->drop();
    rubricRowTable->create();

    DatabaseTable* studentTable = new DatabaseTable(database, "Students", "id TEXT, name TEXT, lafayette_username TEXT, class TEXT");
    studentTable->drop();
    studentTable->create();

    for(GAClass* c: this->classes) {
        c->save_to(classesTable);
        for(GAStudent* s: c->get_students()) {
            s->save_to(studentTable);

            for (auto const& x: s->get_map()) {
//                x.second->save_to(assignmentDataTable);
            }
        }
        for(GAAssignment* a: c->get_assignments()) {
            a->save_to(assignmentTable);
        }
    }

    delete annotationTable;
    delete assignmentTable;
//    delete assignmentDataTable;
    delete classesTable;
    delete rubricTable;
    delete rubricRowTable;
    delete studentTable;

    return true;
}
