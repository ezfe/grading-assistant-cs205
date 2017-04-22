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
    this->annotationTable = new DatabaseTable(database, "Annotations", "id TEXT PRIMARY KEY, assignment_data TEXT, type TEXT, title TEXT, description TEXT, category TEXT, points INT, filename TEXT, line INT");
    this->assignmentTable = new DatabaseTable(database, "Assignments", "id TEXT PRIMARY KEY, title TEXT, description TEXT, class TEXT, rubric TEXT UNIQUE");
    this->assignmentDataTable = new DatabaseTable(database, "AssignmentData", "id TEXT PRIMARY KEY, student TEXT, assignment TEXT, manual_score INT");
    this->classesTable = new DatabaseTable(database, "Classes", "id TEXT PRIMARY KEY, name TEXT");
    this->rubricTable = new DatabaseTable(database, "Rubrics", "id TEXT PRIMARY KEY, title TEXT");
    this->rubricRowTable = new DatabaseTable(database, "RubricRows", "id TEXT PRIMARY KEY, category TEXT, total_points INT, rubric TEXT, extra_credit INT");
    this->rubricRowValuesTable = new DatabaseTable(database, "RubricRowValues", "id INT PRIMARY KEY, value TEXT, rubric_row TEXT");
    this->studentTable = new DatabaseTable(database, "Students", "id TEXT PRIMARY KEY, name TEXT, lafayette_username TEXT, class TEXT");
}

/*!
 * \brief Deconstruct the GradingAssistant object
 *
 * This will delete all the classes and tables from memory
 */
GradingAssistant::~GradingAssistant() {
    for(GAClass* class_: this->classes) {
        delete class_;
    }
    this->classes.clear();

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
    c->save(true);
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
    c->remove();
    delete c;
}

/*!
 * \brief Get the GARubric vector
 * \return The GARubric vector
 */
std::vector<GARubric*> GradingAssistant::get_rubrics() {
    std::vector<GARubric*> rubrics;
    for(GAClass* class_: this->get_classes()) {
        for(GAAssignment* assignment: class_->get_assignments()) {
            rubrics.push_back(assignment->get_rubric());
        }
    }
    return rubrics;
}

/*!
 * \brief Get a specific GARubric
 * \param identifier The persistence identifier of the rubric
 * \return The rubric
 */
GARubric* GradingAssistant::get_rubric(std::string identifier) {
    std::vector<GARubric*> rubrics = this->get_rubrics();

    for(GARubric* r: rubrics) {
        if (r->get_id() == identifier) {
            return r;
        }
    }
    return nullptr;
}

/*!
 * \brief Sort annotations
 * \param left First annotation
 * \param right Second annotatoin
 * \return Last annotation
 */
bool annotation_sorter(std::pair<GAAnnotation*, int> left, std::pair<GAAnnotation*, int> right) {
    return left.second > right.second;
}

/*!
 * \brief Get a student
 * \param identifier Either the lafayette ID, or the persistence identifier
 * \return The student
 */
GAStudent* GradingAssistant::get_student(std::string identifier) {
    for(GAClass* class_: this->get_classes()) {
        for(GAStudent* student: class_->get_students()) {
            if (student->get_id() == identifier) {
                return student;
            } else if (student->get_lafayette_username() == identifier) {
                return student;
            }
        }
    }
    return nullptr;
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

    std::sort(scores.begin(), scores.end(), annotation_sorter);

    std::vector<GAAnnotation*> return_found;

    if (scores.size() <= 0) {
        return return_found;
    }

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
    this->annotationTable->recreate();
    this->assignmentTable->recreate();
    this->assignmentDataTable->recreate();
    this->classesTable->recreate();
    this->rubricTable->recreate();
    this->rubricRowTable->recreate();
    this->rubricRowValuesTable->recreate();
    this->studentTable->recreate();

    /* Loop through the classes */
    for(GAClass* c: this->classes) {
        c->save(true);
    }
}

/*!
 * \brief Load the data
 *
 * This will not currently clear the existing data in memory. The GradingAssistant
 * object should be initialized prior to running this.
 */
void GradingAssistant::load() {
    std::vector<GAClass*> classes = GAClass::load(this);
    for(GAClass* c: classes) {
        this->add_class(c);

        std::cout << "Loading class " << c->get_name() << std::endl;

        std::cout << "  Loading assignments vector" << std::endl;
        std::vector<GAAssignment*> assignments = GAAssignment::load(this, c);
        std::cout << "  Loaded assignments vector" << std::endl;
        for(GAAssignment* a: assignments) {
            std::cout << "  Loaded assignment " << a->get_title() << std::endl;
            c->add_assignment(a);
        }

        std::vector<GAStudent*> students = GAStudent::load(this, c);
        for(GAStudent* s: students) {
            c->add_student(s);

            std::cout << "  Loading student " << s->get_name() << std::endl;

            for(GAAssignment* a: assignments) {
                GAAssignmentData* d = GAAssignmentData::load(this, a, s);
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
