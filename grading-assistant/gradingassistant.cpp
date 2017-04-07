#include "gradingassistant.h"

GradingAssistant::GradingAssistant(DatabaseManager* database) {
    this->database = database;
    this->annotationTable = new DatabaseTable(database, "Annotations", "id TEXT NOT NULL UNIQUE, assignment_data TEXT, type TEXT, title TEXT, description TEXT, category TEXT, location TEXT");
    this->assignmentTable = new DatabaseTable(database, "Assignments", "id TEXT NOT NULL UNIQUE, title TEXT, description TEXT, class TEXT");
    this->assignmentDataTable = new DatabaseTable(database, "AssignmentData", "id TEXT NOT NULL UNIQUE, student TEXT, assignment TEXT");
    this->classesTable = new DatabaseTable(database, "Classes", "id TEXT NOT NULL UNIQUE, name TEXT");
    this->rubricTable = new DatabaseTable(database, "Rubrics", "id TEXT NOT NULL UNIQUE, title TEXT, max_points INTEGER");
    this->rubricRowTable = new DatabaseTable(database, "RubricRows", "id TEXT NOT NULL UNIQUE, category TEXT, total_points INT, rubric TEXT, extra_credit INT");
    this->rubricRowValuesTable = new DatabaseTable(database, "RubricRowValues", "id TEXT NOT NULL, value TEXT, rubric_row TEXT");
    this->studentTable = new DatabaseTable(database, "Students", "id TEXT NOT NULL UNIQUE, name TEXT, lafayette_username TEXT, class TEXT");
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

std::vector<GARubric*> GradingAssistant::get_rubrics() {
    return this->rubrics;
}

void GradingAssistant::add_rubric(GARubric *r) {
    this->rubrics.push_back(r);
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

    this->rubricRowValuesTable->drop();
    this->rubricRowValuesTable->create();

    this->studentTable->drop();
    this->studentTable->create();

    for(GARubric* r: this->rubrics) {
        std::cout << "Saved rubric " << r->get_title() << std::endl;
        r->save_to(this->rubricTable);
        for(GARubricRow* row: r->get_rows()) {
            std::cout << "  Saved rubric row " << row->get_category() << std::endl;
            row->save_to(this->rubricRowTable, this->rubricRowValuesTable);
        }
        if (r->get_ec() != nullptr) {
            r->get_ec()->save_to(this->rubricRowTable, this->rubricRowValuesTable);
        }
    }

    for(GAClass* c: this->classes) {
        c->save_to(this->classesTable);

        std::cout << "Saving class " << c->get_name() << std::endl;

        for(GAAssignment* a: c->get_assignments()) {
            a->save_to(this->assignmentTable);

            std::cout << "  Saved assignment " << a->get_title() << std::endl;
        }
        for(GAStudent* s: c->get_students()) {
            s->save_to(this->studentTable);

            std::cout << "  Saving student " << s->get_name() << std::endl;

            for (auto const& x: s->get_map()) {
                x.second->save_to(this->assignmentDataTable);

                for(GAAnnotation* annot: x.second->get_annotations()) {
                    annot->save_to(this->annotationTable);
                }
            }

            std::cout << "  Saved student " << s->get_name() << std::endl;
        }

        std::cout << "Finished saving class " << c->get_name() << std::endl;
    }

    return true;
}

bool GradingAssistant::load() {
    std::vector<GARubric*> rubrics = GARubric::load_from(this->rubricTable);
    for(GARubric* r: rubrics) {
        this->add_rubric(r);

        std::cout << "Loading rubric " << r->get_title() << std::endl;

        std::vector<GARubricRow*> rows = GARubricRow::load_from(this->rubricRowTable, this->rubricRowValuesTable, r);
        for(GARubricRow* row: rows) {
            std::cout << "  Loading rubric row " << row->get_category() << std::endl;
            if (row->is_extra_credit()) {
                std::cout << "    Extra Credit..." << std::endl;
                r->set_ec(row);
            } else {
                std::cout << "    Regular..." << std::endl;
                r->add_row(row);
            }
        }

        std::cout << "Finished rubric " << r->get_title() << std::endl;
    }

    std::vector<GAClass*> classes = GAClass::load_from(this->classesTable);
    for(GAClass* c: classes) {
        this->add_class(c);

        std::cout << "Loading class " << c->get_name() << std::endl;

        std::vector<GAAssignment*> assignments = GAAssignment::load_from(this->assignmentTable, c);
        for(GAAssignment* a: assignments) {

            std::cout << "  Loaded assignment " << a->get_title() << std::endl;

            c->add_assignment(a);
        }

        std::vector<GAStudent*> students = GAStudent::load_from(this->studentTable, c);
        for(GAStudent* s: students) {
            c->add_student(s);

            std::cout << "  Loading student " << s->get_name() << std::endl;

            for(GAAssignment* a: assignments) {
                GAAssignmentData* d = GAAssignmentData::load_from(this->assignmentDataTable, a, s);
                if (d != nullptr) {
                    std::cout << "    Loading assignment data for " << a->get_title() << std::endl;
                    s->set_data(a, d);

                    std::vector<GAAnnotation*> annots = GAAnnotation::load_from(this->annotationTable, d);
                    for(GAAnnotation* annot: annots) {
                        std::cout << "      Loaded annotation " << annot->get_title() << " for " << a->get_title() << std::endl;
                        d->add_annotation(annot);
                    }
                    std::cout << "    Finished assignment data for " << a->get_title() << std::endl;
                }
            }

            std::cout << "  Finished student " << s->get_name() << std::endl;
        }

        std::cout << "Finished class " << c->get_name() << std::endl;
    }
    return true;
}
