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
    DatabaseManager* database = new DatabaseManager(FileManager::get_database_path());
    ga = new GradingAssistant(database);

    database->open();
    ga->load();

    rd = nullptr;
    ssd = nullptr;
    gs = nullptr;
    aad = nullptr;
    asd = nullptr;

    setup_shortcuts();
}


/*!
 * @brief Destructs everything created for this dialog
 */
BaseScreen::~BaseScreen() {
    delete ui;
}


void BaseScreen::setup_shortcuts() {

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
    }
    else if(ui->stackedWidget->currentIndex() == 4) //at rubrics
    {
        ui->stackedWidget->setCurrentIndex(0); //go back to mainscreen
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
        gs->exec();

        delete gs;
    }
}


/**
 * @brief BaseScreen::on_actionQuit_triggered handles activities of Quit button.
 *
 * Gracefully exits the program.
 */
void BaseScreen::on_actionQuit_triggered()
{
    this->ga->save();
    close();
}


/**
 * @brief BaseScreen::on_temp_save_clicked saves the entire GradingAssistant object.
 */
void BaseScreen::on_temp_save_clicked() {
    this->ga->save();
}


/**
 * @brief BaseScreen::on_selectFilePathButton_clicked opens a file dialog to
 * allow users to select a zip file of student programs.
 */
void BaseScreen::on_selectFilePathButton_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this,
                                                    tr("Open File"),
                                                    "C://",
                                                    tr("Zip Files(*zip)"));
    if(!filePath.isEmpty()) {
        ui->fileEdit->setText(filePath);
    }
}


/**
 * @brief BaseScreen::on_importButton_clicked gets the file path the user has
 * inputted.
 */
void BaseScreen::on_importButton_clicked()
{
    QString filePath = ui->fileEdit->text();
    ui->fileEdit->clear();

    ssd = new SetupSessionDialog(this, ga);
    ssd->exec();

    //start_grading(ssd->get_selected_class(), ssd->get_selected_rubric(),
    //              ssd->get_selected_assignment());
    //open files using given filePath and open grading session

    delete ssd;
}



//CLASS PAGE (PAGE 1) SLOTS



void BaseScreen::on_classListWidget_itemDoubleClicked(QListWidgetItem *item)
{
    //navigate to correct page
    selectedClass = ga->get_classes()[item->listWidget()->currentRow()];
    ui->stackedWidget->setCurrentIndex(2);

    //clear, then fill student list
    ui->studentListWidget->clear();

    for(int i = 0; i < selectedClass->get_students().size(); i++) {
        QListWidgetItem *item = new QListWidgetItem;
        item->setText(QString::fromStdString(selectedClass->get_students()[i]->
                                             get_name().c_str()));
        ui->studentListWidget->addItem(item);
    }

    //clear, then fill assignment list
    ui->assignmentListWidget->clear();

    for(int j = 0; j < selectedClass->get_assignments().size(); j++) {
        QListWidgetItem *item = new QListWidgetItem;
        item->setText(QString::fromStdString(selectedClass->get_assignments()[j]->
                                             get_title().c_str()));
        ui->assignmentListWidget->addItem(item);
    }
}


/**
 * @brief BaseScreen::on_deleteButton_clicked
 */
void BaseScreen::on_deleteButton_clicked() {
    selectedClass = ga->get_classes()[ui->classListWidget->currentRow()];
    selectedClass->remove();
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
        GAClass *newClass = new GAClass(newClassTitle.toStdString());
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
    ui->studentClassLabel->setText(QString::fromStdString(selectedClass->get_name()));
    //set grade label

    ui->pastAssignmentsWidget->clear();

    std::map<GAAssignment*, GAAssignmentData*> assignmentMap = selectedStudent->get_map();

    for(std::map<GAAssignment*, GAAssignmentData*>::iterator itr = assignmentMap.begin();
        itr != assignmentMap.end(); itr++)
    {
        QListWidgetItem *item = new QListWidgetItem;
        std::string label = itr->first->get_title() + ": " + std::to_string(itr->second->
                     calculate_score()) + "/" + std::to_string(itr->first->get_rubric()->get_max_points());
        item->setText(QString::fromStdString(label));
        ui->assignmentListWidget->addItem(item);
    }
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
    asd = new AddStudentDialog(this);
    asd->exec();

    GAStudent *student = asd->get_new_student();

    delete asd;

    //As long as the user has inputted appropriate values, add to class and list
    if(student == nullptr) {
        return;
    }
    else {
        student->set_class(selectedClass);
        selectedClass->add_student(student);

        //clear, then refill student list
        ui->studentListWidget->clear();
        for(int i = 0; i < selectedClass->get_students().size(); i++) {
            QListWidgetItem *item = new QListWidgetItem;
            item->setText(QString::fromStdString(selectedClass->get_students()[i]->
                                                 get_name().c_str()));
            ui->studentListWidget->addItem(item);
        }
    }
}


/**
 * @brief BaseScreen::on_assignmentListWidget_itemDoubleClicked
 * @param item
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
        assignment->set_class(selectedClass);

        selectedClass->add_assignment(assignment);

        //clear, then refill assignment list
        ui->assignmentListWidget->clear();
        for(int j = 0; j < selectedClass->get_assignments().size(); j++) {
            QListWidgetItem *item = new QListWidgetItem;
            item->setText(QString::fromStdString(selectedClass->get_assignments()[j]->
                                                 get_title().c_str()));
            ui->assignmentListWidget->addItem(item);
        }
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

    ui->rubricLabel->setText(QString::fromStdString(selectedAssignment
                                                    ->get_rubric()->get_title()));
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
    rd->exec();

    item->setText(QString::fromStdString(selectedRubric->get_title()));

    delete rd;
}
