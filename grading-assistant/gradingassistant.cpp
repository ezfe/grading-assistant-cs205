#include "gradingassistant.h"

/*!
 * \brief Create a GradingAssistant
 *
 * The initalizer defines all the tables, but does not
 * load anything
 *
 * \param database The database to use
 */
GradingAssistant::GradingAssistant(DatabaseManager* database) {
    this->database = database;
    this->annotationTable = new DatabaseTable(database, "Annotations", "id TEXT NOT NULL UNIQUE, assignment_data TEXT, type TEXT, title TEXT, description TEXT, category TEXT, location TEXT");
    this->assignmentTable = new DatabaseTable(database, "Assignments", "id TEXT NOT NULL UNIQUE, title TEXT, description TEXT, class TEXT, rubric TEXT");
    this->assignmentDataTable = new DatabaseTable(database, "AssignmentData", "id TEXT NOT NULL UNIQUE, student TEXT, assignment TEXT, manual_score INT");
    this->classesTable = new DatabaseTable(database, "Classes", "id TEXT NOT NULL UNIQUE, name TEXT");
    this->rubricTable = new DatabaseTable(database, "Rubrics", "id TEXT NOT NULL UNIQUE, title TEXT");
    this->rubricRowTable = new DatabaseTable(database, "RubricRows", "id TEXT NOT NULL UNIQUE, category TEXT, total_points INT, rubric TEXT, extra_credit INT");
    this->rubricRowValuesTable = new DatabaseTable(database, "RubricRowValues", "id TEXT NOT NULL, value TEXT, rubric_row TEXT");
    this->studentTable = new DatabaseTable(database, "Students", "id TEXT NOT NULL UNIQUE, name TEXT, lafayette_username TEXT, class TEXT");
}

/*!
 * \brief Deconstruct the GradingAssistant object
 *
 * This will delete all the classes, rubrics, and tables from memory
 */
GradingAssistant::~GradingAssistant() {
    for(GAClass* aClass: this->classes) {
        delete aClass;
    }
    this->classes.clear();

    for(GARubric* r: this->rubrics) {
        delete r;
    }
    this->rubrics.clear();

    delete annotationTable;
    delete assignmentTable;
    delete assignmentDataTable;
    delete classesTable;
    delete rubricTable;
    delete rubricRowTable;
    delete studentTable;
}

/*!
 * \brief Get the GAClass vector
 * \return The GAClass vector
 */
std::vector<GAClass*> GradingAssistant::get_classes() {
    return this->classes;
}

/*!
 * \brief Get a specific GAClass
 * \param identifier The persistence identifier of the class
 * \return The class
 */
GAClass* GradingAssistant::get_class(std::string identifier) {
    for(GAClass* c: this->classes) {
        if (c->get_id() == identifier) {
            return c;
        }
    }
    return nullptr;
}

/*!
 * \brief Add a GAClass
 * \param c The GAClass to add
 */
void GradingAssistant::add_class(GAClass* c) {
    c->set_grading_assistant(this);
    this->classes.push_back(c);
}

/*!
 * \brief Remove a GAClass
 *
 * The class will be removed from memory
 *
 * \param c The class to remove
 */
void GradingAssistant::remove_class(GAClass* c) {
    std::vector<GAClass*> current = this->classes;
    this->classes.clear();
    for(GAClass* check_class: current) {
        if (check_class != c) {
            this->classes.push_back(check_class);
        }
    }
    delete c;
}

/*!
 * \brief Get the GARubric vector
 * \return The GARubric vector
 */
std::vector<GARubric*> GradingAssistant::get_rubrics() {
    return this->rubrics;
}

/*!
 * \brief Get a specific GARubric
 * \param identifier The persistence identifier of the rubric
 * \return The rubric
 */
GARubric* GradingAssistant::get_rubric(std::string identifier) {
    for(GARubric* r: this->rubrics) {
        if (r->get_id() == identifier) {
            return r;
        }
    }
    return nullptr;
}


/*!
 * \brief Add a GARubric
 * \param r The GARubric to add
 */
void GradingAssistant::add_rubric(GARubric* r) {
    this->rubrics.push_back(r);
}

/*!
 * \brief Remove a GARubric
 *
 * The rubric will be removed from memory
 *
 * \param c The rubric to remove
 */
void GradingAssistant::remove_rubric(GARubric* r) {
    std::vector<GARubric*> current = this->rubrics;
    this->rubrics.clear();
    for(GARubric* check_rubric: current) {
        if (check_rubric != r) {
            this->rubrics.push_back(check_rubric);
        }
    }
    delete r;
}

bool sorter(std::pair<GAAnnotation*, int> left, std::pair<GAAnnotation*, int> right) {
    return left.second > right.second;
}

/*!
 * \brief Get annotations from a search parameter
 * \param search The query to search by
 * \return The list of annotations
 */
std::vector<GAAnnotation*> GradingAssistant::query_annotation(std::string search) {
    std::vector<std::string> search_terms;
    std::stringstream stream(search);
    std::string res;
    while (std::getline(stream, res, ' ')) {
        std::transform(res.begin(), res.end(), res.begin(), ::tolower);
        search_terms.push_back(res);
    }

    std::vector<std::pair<GAAnnotation*, int>> scores;

    for(GAClass* class_: this->classes) {
        for(GAStudent* student: class_->get_students()) {
            for (auto const& x: student->get_map()) {
                std::vector<GAAnnotation*> annots = x.second->get_annotations();
                for(GAAnnotation* annot: annots) {
                    int score = 0;
                    for(std::string term: search_terms) {
                        std::string title = annot->get_title();
                        std::transform(title.begin(), title.end(), title.begin(), ::tolower);
                        std::string description = annot->get_description();
                        std::transform(description.begin(), description.end(), description.begin(), ::tolower);
                        if (title.find(term) != std::string::npos) {
                            score++;
                        } else if (description.find(term) != std::string::npos) {
                            score++;
                        }
                    }
                    scores.push_back(std::pair<GAAnnotation*, int>(annot, score));
                }
            }
        }
    }

    std::sort(scores.begin(), scores.end(), sorter);

    std::vector<GAAnnotation*> return_found;
    int i = 0;
    int last = scores.front().second;
    for(std::pair<GAAnnotation*, int> pair: scores) {
        if (i > 10) {
            break;
        }
        if (pair.second < last) {
            i++;
            last = pair.second;
        }
        if (pair.second > 0) {
            return_found.push_back(pair.first);
        }
    }

    return return_found;
}

/*!
 * \brief Save all the data
 *
 * This will clear all the tables, then go through all the objects and save them
 */
void GradingAssistant::save() {
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

    /* Loop through the rubrics */
    for(GARubric* r: this->rubrics) {
        std::cout << "Saved rubric " << r->get_title() << std::endl;

        /* Save the rubric */
        r->save_to(this->rubricTable);

        /* Loop through the rows in the rubric */
        for(GARubricRow* row: r->get_rows()) {
            std::cout << "  Saved rubric row " << row->get_category() << std::endl;

            /* Save the row */
            row->save_to(this->rubricRowTable, this->rubricRowValuesTable);
        }

        /* Check for extra credit */
        if (r->get_ec() != nullptr) {
            /* Save the extra credit */
            r->get_ec()->save_to(this->rubricRowTable, this->rubricRowValuesTable);
        }
    }

    /* Loop through the classes */
    for(GAClass* c: this->classes) {
        /* Save the class */
        c->save_to(this->classesTable);

        std::cout << "Saving class " << c->get_name() << std::endl;

        /* Loop through the assignments */
        for(GAAssignment* a: c->get_assignments()) {
            /* Save the assignment */
            a->save_to(this->assignmentTable);

            std::cout << "  Saved assignment " << a->get_title() << std::endl;
        }

        /* Loop through the students */
        for(GAStudent* s: c->get_students()) {
            /* Save the student */
            s->save_to(this->studentTable);

            std::cout << "  Saving student " << s->get_name() << std::endl;

            /* Loop through the assignment data objects */
            for (auto const& x: s->get_map()) {
                /* Save the assignment data to the table */
                x.second->save_to(this->assignmentDataTable);

                /* Loop through the annotations */
                for(GAAnnotation* annot: x.second->get_annotations()) {
                    /* Save the annotation */
                    annot->save_to(this->annotationTable);
                }
            }

            std::cout << "  Saved student " << s->get_name() << std::endl;
        }

        std::cout << "Finished saving class " << c->get_name() << std::endl;
    }
}

/*!
 * \brief Load the data
 *
 * This will not currently clear the existing data in memory. The GradingAssistant
 * object should be initialized prior to running this.
 */
void GradingAssistant::load() {
    std::vector<GARubric*> rubrics = GARubric::load_from(this->rubricTable, this->rubricRowTable, this->rubricRowValuesTable);
    for(GARubric* r: rubrics) {
        this->add_rubric(r);

        std::cout << "Loaded Rubric " << r->get_title() << std::endl;
    }

    std::vector<GAClass*> classes = GAClass::load_from(this->classesTable);
    for(GAClass* c: classes) {
        this->add_class(c);

        std::cout << "Loading class " << c->get_name() << std::endl;

        std::vector<GAAssignment*> assignments = GAAssignment::load_from(this->assignmentTable, this->rubricTable, this->rubricRowTable, this->rubricRowValuesTable, c);
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
}
