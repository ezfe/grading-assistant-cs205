#include "gradingassistant.h"

GradingAssistant::GradingAssistant(DatabaseManager* database) {
    this->database = database;

    this->annotationTable = new DatabaseTable(database, "Annotations", "id TEXT, type TEXT, title TEXT, description TEXT, category TEXT, location TEXT");
    this->assignmentTable = new DatabaseTable(database, "Assignments", "id TEXT, title TEXT, description TEXT, class TEXT");
    this->assignmentDataTable = new DatabaseTable(database, "AssignmentData", "id TEXT, student TEXT, assignment TEXT");
    this->classesTable = new DatabaseTable(database, "Classes", "id TEXT, name TEXT");
    this->rubricTable = new DatabaseTable(database, "Rubrics", "id TEXT, title TEXT, max_points INTEGER");
    this->rubricRowTable = new DatabaseTable(database, "RubricRows", "id TEXT, category TEXT, rubric TEXT");
    this->studentTable = new DatabaseTable(database, "Students", "id TEXT, name TEXT, lafayette_username TEXT, class TEXT");
}

GradingAssistant::~GradingAssistant() {
    for(GAClass* aClass: this->classes) {
        delete aClass;
    }

    delete annotationTable;
    delete assignmentTable;
    delete assignmentDataTable;
    delete classesTable;
    delete rubricTable;
    delete rubricRowTable;
    delete studentTable;
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

bool GradingAssistant::save() {
    this->annotationTable->drop();
    this->annotationTable->create();

    this->assignmentTable->drop();
    this->assignmentTable->create();

    this->assignmentDataTable->drop();
    this->assignmentDataTable->create();

    this->classesTable->drop();
    this->classesTable->create();

    this->rubricTable->drop();
    this->rubricTable->create();

    this->rubricRowTable->drop();
    this->rubricRowTable->create();

    this->studentTable->drop();
    this->studentTable->create();

    for(GAClass* c: this->classes) {
        c->save_to(classesTable);
        for(GAAssignment* a: c->get_assignments()) {
            a->save_to(assignmentTable);
        }
        for(GAStudent* s: c->get_students()) {
            s->save_to(studentTable);

            for (auto const& x: s->get_map()) {
                x.second->save_to(assignmentDataTable);
            }
        }
    }

    return true;
}

bool GradingAssistant::load() {
    std::vector<GAClass*> classes = GAClass::load_from(this->classesTable);
    for(GAClass* c: classes) {
        this->add_class(c);

        std::vector<GAAssignment*> assignments = GAAssignment::load_from(this->assignmentTable, c);
        for(GAAssignment* a: assignments) {
            c->add_assignment(a);
        }

        std::vector<GAStudent*> students = GAStudent::load_from(this->studentTable, c);
        for(GAStudent* s: students) {
            c->add_student(s);

            std::cout << "Loaded student " << s->get_name() << std::endl;

            for(GAAssignment* a: assignments) {
                GAAssignmentData* d = GAAssignmentData::load_from(this->assignmentDataTable, a, s);
                if (d != nullptr) {
                    s->set_data(a, d);
                }
            }
        }
    }
    return true;
}
