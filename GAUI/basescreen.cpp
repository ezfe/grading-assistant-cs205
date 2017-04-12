#include "basescreen.h"
#include "ui_basescreen.h"


BaseScreen::BaseScreen(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::BaseScreen)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);

    FileManager::assure_directory_exists(FileManager::get_app_directory());
    DatabaseManager* database = new DatabaseManager(FileManager::get_database_path());
    ga = new GradingAssistant(database);

    database->open();
    ga->load();

    //    GAClass * myClass = new GAClass("CS 150");
    //    myClass->add_assignment(new GAAssignment("CS 150"));
    //    myClass->add_student(new GAStudent("Natalie Sampsell"));
    //    ga->add_class(myClass);

    rd = nullptr;
}

BaseScreen::~BaseScreen() {
    delete ui;
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
    GARubric *testRubric = new GARubric("Test Rubric");
    std::vector<std::string> descrips;
    descrips.push_back("Bad");
    descrips.push_back("Ok");
    descrips.push_back("Good");
    testRubric->add_row("Style", descrips, 5);
    testRubric->add_row("Correctness", descrips, 5);
    testRubric->add_row("Design", descrips, 5);
    testRubric->add_row("Completeness", descrips, 5);
    testRubric->set_ec("Extra Credit", "credz 4 u", 2);

    ga->add_rubric(testRubric);

    gs = new GradingSession(this, ga, ga->get_classes()[0], ga->get_rubrics().back(), ga->get_classes()[0]->get_assignments()[0]);
    gs->exec();
}

/**
 * @brief BaseScreen::on_actionQuit_triggered handles activities of Quit button.
 *
 * Gracefully exits the program.
 */
void BaseScreen::on_actionQuit_triggered()
{
    //save and quit
    close();
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

    start_grading(ssd->get_selected_class(), ssd->get_selected_rubric(),
                  ssd->get_selected_assignment());
    //open files using given filePath and open grading session
}

//CLASSES PAGE (PAGE 1) SLOTS

void BaseScreen::on_deleteButton_clicked()
{
    //implement delete
}

/**
 * @brief BaseScreen::on_selectButton_clicked handles activities of Select button on
 * the Classes page.
 *
 * Sets up the assignments/classes page based on which class the user has selected.
 */
void BaseScreen::on_selectButton_clicked()
{
    //navigate to correct page
    selectedClass = ga->get_classes()[ui->classListWidget->currentRow()];
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

void BaseScreen::on_addStudentButton_clicked()
{
    //add dialog
}

/**
 * @brief BaseScreen::on_selectStudentButton_clicked handles activities of the Select
 * Student button on the Students/Assignments page.
 *
 * Allows users to view the profile for the student they have selected.
 */
void BaseScreen::on_selectStudentButton_clicked()
{
    selectedStudent = selectedClass->get_students()[ui->studentListWidget->currentRow()];
    ui->stackedWidget->setCurrentIndex(3);

    ui->studentNameLabel->setText(QString::fromStdString(selectedStudent->get_name()));
    ui->studentClassLabel->setText(QString::fromStdString(selectedClass->get_name()));
    //set grade label

    //set up list widget with student assignment data
}

void BaseScreen::on_addNewAssignmentButton_clicked()
{
    //add dialog
}

/**
 * @brief BaseScreen::on_selectAssignmentButton_clicked handles activities of the Select
 * Assignment button on the Students/Assignments page.
 *
 * Allows users to view the assignment they have selected.
 */
void BaseScreen::on_selectAssignmentButton_clicked() //MAKE IMPOSSIBLE TO CHANGE IF NO ASSN SELECTED
{
    selectedAssignment = selectedClass->get_assignments()[ui->assignmentListWidget->currentRow()];
    ui->stackedWidget->setCurrentIndex(5);

    //fill in title + description
    ui->titleEdit->setText(QString::fromStdString(selectedAssignment->get_title()));
    ui->titleEdit->setReadOnly(true);
    ui->descriptionEdit->clear();
    ui->descriptionEdit->appendPlainText(QString::fromStdString(selectedAssignment->
                                                                get_description()));
    ui->descriptionEdit->setReadOnly(true);
}

//ASSIGNMENT PAGE (PAGE 5) SLOTS

/**
 * @brief BaseScreen::on_editButton_clicked sets the text edits on the assignment page
 * to editable.
 */
void BaseScreen::on_editButton_clicked()
{
    ui->titleEdit->setReadOnly(false);
    ui->descriptionEdit->setReadOnly(false);
}

/**
 * @brief BaseScreen::on_saveButton_clicked updates the variables associated with
 * the edited assignment.
 */
void BaseScreen::on_saveButton_clicked()
{
    selectedAssignment->set_title(ui->titleEdit->text().toStdString());
    ui->titleEdit->setReadOnly(true);
    selectedAssignment->set_description(ui->descriptionEdit->toPlainText().toStdString());
    ui->descriptionEdit->setReadOnly(true);

    ui->stackedWidget->setCurrentIndex(2); //go back to students/assignments
}

//RUBRICS PAGE (PAGE 4) SLOTS

/**
 * @brief BaseScreen::on_createButton_clicked handles the activities of the Create
 * button on the Rubrics page.
 *
 * Opens a dialog to allow users to create a rubric using the values the user has inputted.
 */
void BaseScreen::on_createButton_clicked()
{

    rd = new RubricDialog(this, ui->rubricTitleEdit->text(),
                          ui->rowsEdit->value(),
                          ui->columnsEdit->value(),
                          ui->pointsEdit->value());
    ui->rubricTitleEdit->clear();
    ui->rowsEdit->setValue(0);
    ui->columnsEdit->setValue(0);
    ui->pointsEdit->setValue(0);
    rd->exec();
    GARubric *newRubric = rd->get_rubric();

    if(newRubric == nullptr) { //user canceled
        return;
    }
    else {
        ga->add_rubric(newRubric);
        QListWidgetItem *item = new QListWidgetItem;
        item->setText(QString::fromStdString(newRubric->get_title()));
        ui->rubricListWidget->addItem(item);
    }

    delete rd;
}

/**
 * @brief BaseScreen::on_selectRubricButton_clicked handles activities of the Select
 * button on the Rubrics page.
 *
 * Allows users to view the rubric they have selected.
 */
void BaseScreen::on_selectRubricButton_clicked()
{
    selectedRubric = ga->get_rubrics()[ui->rubricListWidget->currentRow()];

    rd = new RubricDialog(this, selectedRubric);
    rd->exec();

    delete rd;
}

void BaseScreen::start_grading(GAClass *c, GARubric *r, GAAssignment *a)
{

}
