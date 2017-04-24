#include "basescreen.h"
#include "ui_basescreen.h"

/**
 * @brief BaseScreen::BaseScreen sets up the main window.
 *
 * @param parent - inherits from QWidget
 */
BaseScreen::BaseScreen(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::BaseScreen)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);


    //Load Database
    FileManager::assure_directory_exists(FileManager::get_app_directory());
    UserSettings* settings = new UserSettings(FileManager::get_settings_path());

    //Configure and initialize server
    if (false) {
        cs = new ConfigureSettings(this);
        cs->exec();

        /* Prompt user... */
//        settings->set("ssh_username", cs->get_username());
//        settings->set("ssh_hostname", cs->get_hostname());
//        settings->set("git_path", cs->get_path());
//        settings->set("git_configured", 1);
        settings->save();

        delete cs;
    }

    serverHandler = new GitHandler("spr2017_l2g4", "139.147.9.185", "/home/spr2017_l2g4/repo_server.git");
    serverHandler->setup();
    serverHandler->sync();
    std::cout << serverHandler->get_errors() << std::endl;

    DatabaseManager* database = new DatabaseManager(FileManager::get_database_path());
    ga = new GradingAssistant(database);

    //Open database and load
    database->open();
    ga->load();

    cs = nullptr;
    rd = nullptr;
    ssd = nullptr;
    gs = nullptr;
    aad = nullptr;
    asd = nullptr;

    deleteTable = false;
    deleteGradebook = false;

    setup_general();
    ui->classListWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->studentListWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->assignmentListWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->classListWidget, SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(show_context_menu_class(QPoint)));
    connect(ui->studentListWidget, SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(show_context_menu_students(QPoint)));
    connect(ui->assignmentListWidget, SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(show_context_menu_assignments(QPoint)));
}


/*!
 * @brief Destructs everything created for this dialog
 */
BaseScreen::~BaseScreen() {
    delete_if_needed();

    //delete items in list widgets
    ui->classListWidget->clear();
    ui->studentListWidget->clear();
    ui->assignmentListWidget->clear();
    ui->rubricListWidget->clear();
    ui->pastAssignmentsWidget->clear();

    this->ga->save();
    this->serverHandler->sync();
    delete ui;
}


void BaseScreen::setup_general() {

}


//WELCOME PAGE (PAGE 0) SLOTS



/**
 * @brief BaseScreen::on_actionBack_triggered handles activities of back button.
 *
 * Allows the user to use a back button to navigate through the
 * program's pages and windows.
 */
void BaseScreen::on_actionBack_triggered()
{
    if(ui->stackedWidget->currentIndex() == 0) //at mainscreen
    {
        return; //at base, do nothing
    }
    else if(ui->stackedWidget->currentIndex() == 1) //at classes
    {
        ui->stackedWidget->setCurrentIndex(0); //go back to mainscreen
    }
    else if(ui->stackedWidget->currentIndex() == 2) //at students/assignments
    {
        ui->stackedWidget->setCurrentIndex(1); //go back to classes
    }
    else if(ui->stackedWidget->currentIndex() == 3) //at student profile
    {
        ui->stackedWidget->setCurrentIndex(2); //go back to students/assignments
    }
    else if(ui->stackedWidget->currentIndex() == 5) //at assignment
    {
        ui->stackedWidget->setCurrentIndex(2); //go back to students/assignments
        delete_if_needed();
    }
    else if(ui->stackedWidget->currentIndex() == 4) //at rubrics
    {
        ui->stackedWidget->setCurrentIndex(0); //go back to mainscreen
    }
    else if(ui->stackedWidget->currentIndex() == 6) //at gradebook
    {
        ui->stackedWidget->setCurrentIndex(2); //go back to students/assignments
        delete_if_needed();
    }
    else
    {
        return;
    }
}


/**
 * @brief BaseScreen::on_actionClasses_triggered handles activties of Classes button.
 *
 * Takes users to a page in the stackedWidget that shows all the classes.
 */
void BaseScreen::on_actionClasses_triggered()
{
    delete_if_needed();

    //switch to correct page
    ui->stackedWidget->setCurrentIndex(1);
    ui->classListWidget->clear();

    //fill list of classes
    for(GAClass* c: ga->get_classes()) {
        QListWidgetItem* item = new QListWidgetItem;
        item->setText(QString::fromStdString(c->get_name()));
        ui->classListWidget->addItem(item);
    }
}


/**
 * @brief BaseScreen::on_actionRubrics_triggered handles activities of Rubrics button.
 *
 * Takes users to a page in the stackedWidget that shows all the rubrics.
 */
void BaseScreen::on_actionRubrics_triggered()
{
    delete_if_needed();

    //switch to correct page
    ui->stackedWidget->setCurrentIndex(4);
    ui->rubricListWidget->clear();

    //fill list of rubrics
    for(GARubric* r: ga->get_rubrics()) {
        QListWidgetItem* item = new QListWidgetItem;
        item->setText(QString::fromStdString(r->get_title()));
        ui->rubricListWidget->addItem(item);
    }
}


/**
 * @brief BaseScreen::on_actionCurrent_Session_triggered handles activties of Current
 * Session button.
 *
 * Takes users to a separate dialog representing a grading session.
 */
void BaseScreen::on_actionCurrent_Session_triggered()
{
    //Get setup information from user
    ssd = new SetupSessionDialog(this, ga);
    ssd->exec();


    //Distribute this information
    GAAssignment *assignment = ssd->get_selected_assignment();
    GAClass* currentClass = ssd->get_selected_class();

    delete ssd;

    //As long as the user has entered valid inputs, open grading session
    if(assignment != nullptr && currentClass != nullptr)
    {
        gs = new GradingSession(this, ga, currentClass,
                                assignment->get_rubric(),
                                assignment);
        gs->setAttribute(Qt::WA_DeleteOnClose);
        gs->show();
    }
}


/*!
 * \brief Another way to import
 */
void BaseScreen::on_actionImport_triggered() {
    this->on_importButton_clicked();
}


/*!
 * @brief BaseScreen::on_actionSave_triggered handles activities of Save button.
 *
 * Saves the grading assistant.
 */
void BaseScreen::on_actionSave_triggered()
{
    this->ga->save();
    this->serverHandler->sync();
}


/**
 * @brief BaseScreen::on_actionQuit_triggered handles activities of Quit button.
 *
 * Gracefully exits the program.
 */
void BaseScreen::on_actionQuit_triggered()
{
    delete_if_needed();
    this->ga->save();
    close();
}

/**
 * @brief BaseScreen::on_importButton_clicked gets the file path the user has
 * inputted.
 */
void BaseScreen::on_importButton_clicked()
{
    QString filePath = QFileDialog::getExistingDirectory(this, tr("Open Directory"), FileManager::get_home().c_str(), QFileDialog::ShowDirsOnly);
    QDir impDir(filePath);
    if (!impDir.exists() || filePath.length() <= 0) {
        std::cerr << "Bad path..." << std::endl;
        return;
    }

    ssd = new SetupSessionDialog(this, ga);
    ssd->exec();

    std::vector<std::string> names = FileManager::import(filePath.toStdString(), ga, ssd->get_selected_assignment());

    for(std::string name: names) {
        std::cout << "Created..." << name << std::endl;
    }

    //    start_grading(ssd->get_selected_class(), ssd->get_selected_rubric(),
    //                  ssd->get_selected_assignment());
    //open files using given filePath and open grading session

    delete ssd;
}



//CLASS PAGE (PAGE 1) SLOTS



/*!
 * @brief Slot called when user double clicks on an item in the class list
 * widget
 * @param item - item that was double clicked
 */
void BaseScreen::on_classListWidget_itemDoubleClicked(QListWidgetItem *item)
{
    //navigate to correct page
    selectedClass = ga->get_classes()[item->listWidget()->currentRow()];
    ui->stackedWidget->setCurrentIndex(2);

    ui->classNameLabel->setText("Class: " + QString::fromStdString(selectedClass->get_name()));

    //clear, then fill student list
    ui->studentListWidget->clear();

    for(GAStudent* student: selectedClass->get_students()) {
        QListWidgetItem* item = new QListWidgetItem;
        item->setText(QString::fromStdString(student->get_name().c_str()));
        ui->studentListWidget->addItem(item);
    }

    //clear, then fill assignment list
    ui->assignmentListWidget->clear();

    for(GAAssignment* assignment: selectedClass->get_assignments()) {
        QListWidgetItem *item = new QListWidgetItem;
        item->setText(QString::fromStdString(assignment->get_title().c_str()));
        ui->assignmentListWidget->addItem(item);
    }
}

/*!
 * @brief Slot called when user right clicks/requests a context menu within the
 * boundaries of the class list widget
 * @param pos - pos of click
 */
void BaseScreen::show_context_menu_class(const QPoint &pos)
{
    // Handle global position
    QPoint globalPos = ui->classListWidget->mapToGlobal(pos);

    // Create menu and insert action
    QMenu myMenu;
    myMenu.addAction("Delete", this, SLOT(delete_class()));

    // Show context menu at handling position
    myMenu.exec(globalPos);
}


/**
 * @brief Deletes the class currently selected in the class list widget
 */
void BaseScreen::delete_class() {

    int ret = QMessageBox::warning(this, tr("Warning"),
                                   tr("You are about to delete this class.\n"
                                      "This will delete all data associated with this class. Are you sure you want to proceed?"),
                                   QMessageBox::Ok | QMessageBox::Cancel,
                                   QMessageBox::Ok);

    if(ret == QMessageBox::Cancel) {
        return;
    }
    else {
        selectedClass = ga->get_classes()[ui->classListWidget->currentRow()];
        ga->remove_class(selectedClass);
        QListWidgetItem *item = ui->classListWidget->takeItem(ui->classListWidget->currentRow());
        delete item;
    }
}


/**
 * @brief BaseScreen::on_addNew_clicked handles activities of the New button
 * on the Classes page.
 *
 * Allows users to make a new class and add it to the list of classes.
 */
void BaseScreen::on_addNew_clicked()
{
    QString newClassTitle = ui->classEdit->text();

    //if the user has inputted a title
    if(!newClassTitle.isEmpty()) {

        //make class
        GAClass* newClass = new GAClass(ga);
        newClass->set_name(newClassTitle.toStdString());

        ga->add_class(newClass);

        //update class list
        QListWidgetItem *item = new QListWidgetItem;
        item->setText(newClassTitle);
        ui->classListWidget->addItem(item);

        //clear edit
        ui->classEdit->clear();
    }

}



//STUDENTS/ASSIGNMENTS PAGE (PAGE 2) SLOTS



/**
 * @brief BaseScreen::on_studentListWidget_itemDoubleClicked handles the activity that
 * results from a user double clicking on the list of students.
 *
 * @param item that was double clicked
 */
void BaseScreen::on_studentListWidget_itemDoubleClicked(QListWidgetItem *item)
{
    selectedStudent = selectedClass->get_students()[item->listWidget()->currentRow()];
    ui->stackedWidget->setCurrentIndex(3);

    //Set up student profile
    ui->studentNameLabel->setText(QString::fromStdString(selectedStudent->get_name()));
    ui->studentNameLabel->setReadOnly(true);
    ui->studentUsernameLabel->setText(QString::fromStdString(selectedStudent->get_lafayette_username()));
    ui->studentUsernameLabel->setReadOnly(true);
    ui->studentClassLabel->setText(QString::fromStdString(selectedClass->get_name()));
    ui->studentClassLabel->setReadOnly(true);
    ui->labGradeLabel->setText(QString::number(selectedStudent->calculate_lab_grade()) + "%");
    ui->labGradeLabel->setReadOnly(true);

    ui->pastAssignmentsWidget->clear();

    std::map<GAAssignment*, GAAssignmentData*> assignmentMap = selectedStudent->get_map();

    currentList.clear();

    for (auto const& x: assignmentMap) {
        QListWidgetItem *item = new QListWidgetItem;
        currentList.push_back(x.second);
        std::string label = x.first->get_title() + ": "
                + std::to_string(x.second->calculate_percentage()) + "%";
        item->setText(QString::fromStdString(label));
        ui->pastAssignmentsWidget->addItem(item);
    }
}


/*!
 * @brief Allow user to edit student name and laf username
 */
void BaseScreen::on_editStudentButton_clicked()
{
    ui->studentNameLabel->setReadOnly(false);
    ui->studentUsernameLabel->setReadOnly(false);
}


/*!
 * @brief Allow user to save changes to student name and laf username
 */
void BaseScreen::on_saveStudentButton_clicked()
{
    ui->studentNameLabel->setReadOnly(true);
    ui->studentUsernameLabel->setReadOnly(true);
    selectedStudent->set_name(ui->studentNameLabel->text().toStdString());
    selectedStudent->set_lafayette_username(ui->studentUsernameLabel->text().toStdString());
    ui->studentListWidget->currentItem()->setText(QString::fromStdString(selectedStudent->get_name()));
}


/*!
 * @brief BaseScreen::on_pastAssignmentsWidget_itemDoubleClicked handles the activity that results
 * from a user double clicking on the list of past assignments
 * @param item
 */
void BaseScreen::on_pastAssignmentsWidget_itemDoubleClicked(QListWidgetItem *item)
{
    std::map<GAAssignment*, GAAssignmentData*> assignmentMap = selectedStudent->get_map();
    GAAssignmentData *selected = currentList[item->listWidget()->currentRow()];

    gd = new GradingDialog(this, selectedStudent, selected->get_assignment()->get_rubric(),
                           selected);
    gd->setAttribute(Qt::WA_DeleteOnClose);
    gd->show();
}


/**
 * @brief BaseScreen::on_addStudentButton_clicked handles activities of the Add
 * button on the Students/Assignments page (associated with student).
 *
 * Allows users to make a new students and add it to the selected class.
 */
void BaseScreen::on_addStudentButton_clicked()
{
    //Open new dialog
    asd = new AddStudentDialog(this, ga);
    asd->exec();

    GAStudent *student = asd->get_new_student();

    delete asd;

    //As long as the user has inputted appropriate values, add to class and list
    if(student == nullptr) {
        return;
    }
    else {
        student->set_grading_assistant(ga);
        student->set_class(selectedClass);
        selectedClass->add_student(student);

        //clear, then refill student list
        ui->studentListWidget->clear();
        for(GAStudent* student: selectedClass->get_students()) {
            QListWidgetItem *item = new QListWidgetItem;
            item->setText(QString::fromStdString(student->get_name().c_str()));
            ui->studentListWidget->addItem(item);
        }
    }
}


/**
 * @brief BaseScreen::on_assignmentListWidget_itemDoubleClicked handles the
 * activity that results from the user double clicking on an item in the
 * assignment list widget
 *
 * @param item - item double clicked
 */
void BaseScreen::on_assignmentListWidget_itemDoubleClicked(QListWidgetItem *item)
{
    selectedAssignment = selectedClass->get_assignments()[item->listWidget()->currentRow()];
    ui->stackedWidget->setCurrentIndex(5);

    //fill in title + description
    ui->titleEdit->setText(QString::fromStdString(selectedAssignment->get_title()));
    ui->titleEdit->setReadOnly(false);
    ui->descriptionEdit->clear();
    ui->descriptionEdit->appendPlainText(QString::fromStdString(selectedAssignment->
                                                                get_description()));
    ui->descriptionEdit->setReadOnly(false);
    ui->rubricLabel->setText(QString::fromStdString("Rubric: "
                                                    + selectedAssignment->get_rubric()->get_title()));

    //set up table widget
    ui->assignmentGradeTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->assignmentGradeTable->verticalHeader()->setVisible(false);

    ui->assignmentGradeTable->setRowCount(selectedClass->get_students().size());
    ui->assignmentGradeTable->setColumnCount(2);

    ui->assignmentGradeTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    QTableWidgetItem *header1 = new QTableWidgetItem(2);
    header1->setText("Student");
    ui->assignmentGradeTable->setHorizontalHeaderItem(0, header1);
    QTableWidgetItem *header2 = new QTableWidgetItem(2);
    header2->setText("Grade");
    ui->assignmentGradeTable->setHorizontalHeaderItem(1, header2);

    //go through all students, fill in grades
    for(int i = 0; i < selectedClass->get_students().size(); i++) {
        QTableWidgetItem *student = new QTableWidgetItem(2);
        student->setText(QString::fromStdString(selectedClass->get_students()[i]->get_name()));
        ui->assignmentGradeTable->setItem(i, 0, student);

        QTableWidgetItem *grade = new QTableWidgetItem(2);
        grade->setText(QString::number(selectedClass->get_students()[i]->get_data(selectedAssignment)->
                                       calculate_percentage()) + "%");
        ui->assignmentGradeTable->setItem(i, 1, grade);
    }

    deleteTable = true;
}


/**
 * @brief BaseScreen::on_addNewAssignmentButton_clicked handles activities of the Add
 * button on the Students/Assignments page (associated with assignment).
 *
 * Allows users to make a new assignment and add it to the selected class.
 */
void BaseScreen::on_addNewAssignmentButton_clicked()
{
    //Open new dialog
    aad = new AddAssignmentDialog(this, ga);
    aad->exec();

    GAAssignment *assignment = aad->get_new_assignment();

    delete aad;

    //As long as the user has inputted appropriate values, add to class and list
    if(assignment == nullptr) {
        return;
    }
    else {
        assignment->set_grading_assistant(ga);
        assignment->set_class(selectedClass);

        selectedClass->add_assignment(assignment);

        //clear, then refill assignment list
        ui->assignmentListWidget->clear();
        for(GAAssignment* assignment: selectedClass->get_assignments()) {
            QListWidgetItem *item = new QListWidgetItem;
            item->setText(QString::fromStdString(assignment->get_title().c_str()));
            ui->assignmentListWidget->addItem(item);
        }
    }
}


/**
 * @brief Slot called when user right clicks/requests a context menu within the boundaries
 * of the student list widget
 * @param pos - position of click
 */
void BaseScreen::show_context_menu_students(const QPoint &pos)
{
    // Handle global position
    QPoint globalPos = ui->studentListWidget->mapToGlobal(pos);

    // Create menu and insert some actions
    QMenu myMenu;
    myMenu.addAction("Delete", this, SLOT(delete_student()));
    myMenu.addAction("Add New", this, SLOT(on_addStudentButton_clicked()));

    // Show context menu at handling position
    myMenu.exec(globalPos);
}


/**
 * @brief Slot called when user right clicks/requests a context menu with the boundaries
 * of the assignment list widget
 * @param pos
 */
void BaseScreen::show_context_menu_assignments(const QPoint &pos) {

    // Handle global position
    QPoint globalPos = ui->assignmentListWidget->mapToGlobal(pos);

    // Create menu and insert some actions
    QMenu myMenu;
    myMenu.addAction("Delete", this, SLOT(delete_assignment()));
    myMenu.addAction("Add New", this, SLOT(on_addNewAssignmentButton_clicked()));

    // Show context menu at handling position
    myMenu.exec(globalPos);
}


/*!
 * @brief Deletes the student currently selected in the student list widget
 */
void BaseScreen::delete_student() {

    int ret = QMessageBox::warning(this, tr("Warning"),
                                   tr("You are about to delete this student.\n"
                                      "This will delete all data associated with this student. Are you sure you want to proceed?"),
                                   QMessageBox::Ok | QMessageBox::Cancel,
                                   QMessageBox::Ok);

    if(ret == QMessageBox::Cancel) {
        return;
    }
    else {
        selectedStudent = selectedClass->get_students()[ui->studentListWidget->currentRow()];
        selectedClass->remove_student(selectedStudent);
        QListWidgetItem *item = ui->studentListWidget->takeItem(ui->studentListWidget->currentRow());
        delete item;
    }
}


/*!
 * @brief Deletes the assignment currently selected in the assignment list widget
 */
void BaseScreen::delete_assignment() {

    int ret = QMessageBox::warning(this, tr("Warning"),
                                   tr("You are about to delete this assignment.\n"
                                      "This will delete all data associated with this assignment. Are you sure you want to proceed?"),
                                   QMessageBox::Ok | QMessageBox::Cancel,
                                   QMessageBox::Ok);

    if(ret == QMessageBox::Cancel) {
        return;
    }
    else {
        selectedAssignment = selectedClass->get_assignments()[ui->assignmentListWidget->currentRow()];
        selectedClass->remove_assignment(selectedAssignment);
        QListWidgetItem *item = ui->assignmentListWidget->takeItem(ui->assignmentListWidget->currentRow());
        delete item;
    }
}



//ASSIGNMENT PAGE (PAGE 5) SLOTS



/**
 * @brief BaseScreen::on_saveButton_clicked updates the variables associated with
 * the edited assignment.
 */
void BaseScreen::on_saveButton_clicked()
{
    selectedAssignment->set_title(ui->titleEdit->text().toStdString());
    selectedAssignment->set_description(ui->descriptionEdit->toPlainText().toStdString());

    ui->stackedWidget->setCurrentIndex(2); //go back to students/assignments
}


/**
 * @brief BaseScreen::on_editRubricButton_clicked lets users edit the rubric associated
 * with the selected assignment.
 */
void BaseScreen::on_editRubricButton_clicked()
{
    rd = new RubricDialog(this, selectedAssignment->get_rubric());
    rd->exec();

    ui->rubricLabel->setText(QString::fromStdString("Rubric: " + selectedAssignment
                                                    ->get_rubric()->get_title()));
}


/*!
 * @brief BaseScreen::delete_assignment_table deletes the table items created
 * when the user views the table of assignment grades
 */
void BaseScreen::delete_assignment_table() {
    delete ui->assignmentGradeTable->horizontalHeaderItem(0);
    delete ui->assignmentGradeTable->horizontalHeaderItem(1);

    int rows = ui->assignmentGradeTable->rowCount();

    for(int i = 0; i < rows; i++) {
        delete ui->assignmentGradeTable->item(i, 0);
        delete ui->assignmentGradeTable->item(i, 1);
    }
}



//RUBRICS PAGE (PAGE 4) SLOTS



/**
 * @brief BaseScreen::on_rubricListWidget_itemDoubleClicked handles the activity that
 * results from a user double clicking on the list of rubrics.
 *
 * Opens up dialog that allows selected rubric to be edited.
 *
 * @param item that was double clicked
 */
void BaseScreen::on_rubricListWidget_itemDoubleClicked(QListWidgetItem *item)
{
    selectedRubric = ga->get_rubrics()[item->listWidget()->currentRow()];

    rd = new RubricDialog(this, selectedRubric);
    rd->show();
    rd->setAttribute(Qt::WA_DeleteOnClose);

    item->setText(QString::fromStdString(selectedRubric->get_title()));
}



//GRADEOOK PAGE (PAGE 6) SLOTS



/*!
 * @brief BaseScreen::on_gradebookButton_clicked handles thea activities of the
 * Gradebook button and sets up the gradebook table.
 */
void BaseScreen::on_gradebookButton_clicked()
{
    //switch to gradebook page
    ui->stackedWidget->setCurrentIndex(6);

    //read-only, set up rows + columns
    ui->gradebookTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->gradebookTableWidget->verticalHeader()->setVisible(false);

    ui->gradebookTableWidget->setRowCount(selectedClass->get_students().size());
    ui->gradebookTableWidget->setColumnCount(selectedClass->get_assignments().size() + 2);

    ui->gradebookTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    //Set up headers
    QTableWidgetItem *header1 = new QTableWidgetItem(2);
    header1->setText("Student");
    ui->gradebookTableWidget->setHorizontalHeaderItem(0, header1);

    for(int i = 1; i < selectedClass->get_assignments().size() + 1; i++) {
        QTableWidgetItem *header = new QTableWidgetItem(2);
        header->setText(QString::fromStdString(selectedClass->get_assignments()[i-1]->get_title()));
        ui->gradebookTableWidget->setHorizontalHeaderItem(i, header);
    }

    QTableWidgetItem *header2 = new QTableWidgetItem(2);
    header2->setText("Overall Grade");
    ui->gradebookTableWidget->setHorizontalHeaderItem(selectedClass->get_assignments().size()
                                                      + 1, header2);

    //Fill table
    for(int i = 0; i < selectedClass->get_students().size(); i++) {

        //student name
        QTableWidgetItem *student = new QTableWidgetItem(2);
        student->setText(QString::fromStdString(selectedClass->get_students()[i]->get_name()));
        ui->gradebookTableWidget->setItem(i, 0, student);

        //assignment grades
        for(int j = 1; j < selectedClass->get_assignments().size()+1; j++) {
            QTableWidgetItem *assignment = new QTableWidgetItem(2);
            assignment->setText(QString::number(selectedClass->get_students()[i]->
                                                       get_data(selectedClass->get_assignments()[j-1])->
                                                       calculate_percentage()) + "%");
            ui->gradebookTableWidget->setItem(i, j, assignment);
        }

        //overall grade
        QTableWidgetItem *grade = new QTableWidgetItem(2);
        grade->setText(QString::number(selectedClass->get_students()[i]->calculate_lab_grade()) + "%");
        ui->gradebookTableWidget->setItem(i, selectedClass->get_assignments().size() + 1, grade);
    }

    deleteGradebook = true;
}


/*!
 * @brief BaseScreen::delete_gradebook_table deletes the contents of the
 * gradebook table created when the user views the gradebook page
 */
void BaseScreen::delete_gradebook_table() {

    int rows = ui->assignmentGradeTable->rowCount();
    int cols = ui->assignmentGradeTable->columnCount();

    for(int i = 0; i < rows; i++) {
        delete ui->gradebookTableWidget->horizontalHeaderItem(i);

        for(int j = 0; j < cols; j++) {
            delete ui->gradebookTableWidget->item(i, j);
        }
    }
}


/*!
 * @brief Aids in memory management by deleting tables that are no longer in use.
 */
void BaseScreen::delete_if_needed() {
    if(deleteTable == true) {
        delete_assignment_table();
        deleteTable = false;
        std::cerr << "deleted assignment table" << std::endl;
    }
    else if(deleteGradebook == true) {
        delete_gradebook_table();
        deleteGradebook = false;
        std::cerr << "deleted gradebook" << std::endl;
    }
    else {
        return;
    }
}


