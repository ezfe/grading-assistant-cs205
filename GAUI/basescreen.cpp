#include "basescreen.h"
#include "ui_basescreen.h"


BaseScreen::BaseScreen(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::BaseScreen)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);

    FileManager::assure_directory_exists(FileManager::get_app_directory());
    DatabaseManager *database = new DatabaseManager(FileManager::get_database_path());
    GradingAssistant* ga = new GradingAssistant(database);

    database->open();

    ga->load();

    GAClass * myClass = new GAClass("CS 150");
    myClass->add_assignment(new GAAssignment("CS 150"));
    myClass->add_student(new GAStudent("Natalie Sampsell"));
    ga->add_class(myClass);
}

BaseScreen::~BaseScreen()
{
    delete ui;
}

//WELCOME PAGE (PAGE 0) SLOTS

void BaseScreen::on_actionBack_triggered()
{
    if(ui->stackedWidget->currentIndex() == 0)
    {
        return;
    }
    else if(ui->stackedWidget->currentIndex() == 1)
    {
        ui->stackedWidget->setCurrentIndex(0);
    }
    else if(ui->stackedWidget->currentIndex() == 2)
    {
        ui->stackedWidget->setCurrentIndex(1);
    }
    else if(ui->stackedWidget->currentIndex() == 3)
    {
        ui->stackedWidget->setCurrentIndex(2);
    }
    else if(ui->stackedWidget->currentIndex() == 4)
    {
        ui->stackedWidget->setCurrentIndex(2);
    }
    else if(ui->stackedWidget->currentIndex() == 5)
    {
        ui->stackedWidget->setCurrentIndex(0);
    }
    else
    {
        return;
    }
}

void BaseScreen::on_actionClasses_triggered()
{
    ui->stackedWidget->setCurrentIndex(1);
    ui->classListWidget->clear();
    for(int i = 0; i < ga->get_classes().size(); i++) {
        QListWidgetItem *item = new QListWidgetItem;
        item->setText(QString::fromStdString(ga->get_classes()[i]->get_name()));
        ui->classListWidget->addItem(item);
    }
}

void BaseScreen::on_actionRubrics_triggered()
{
    ui->stackedWidget->setCurrentIndex(5);
}

void BaseScreen::on_actionCurrent_Session_triggered()
{
    //current session window
}

void BaseScreen::on_actionQuit_triggered()
{
    //save and quit
    close();
}

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

void BaseScreen::on_importButton_clicked()
{
    QString filePath = ui->fileEdit->text();
    ui->fileEdit->clear();
    //open files using given filePath and open grading session
}

//CLASSES PAGE (PAGE 1) SLOTS

void BaseScreen::on_deleteButton_clicked()
{
    //implement delete
}

void BaseScreen::on_selectButton_clicked()
{
    selectedClass = ga->get_classes()[ui->classListWidget->currentRow()];
    ui->stackedWidget->setCurrentIndex(2);
    ui->studentListWidget->clear();
    for(int i = 0; i < selectedClass->get_students().size(); i++) {
        QListWidgetItem *item = new QListWidgetItem;
        item->setText(QString::fromStdString(selectedClass->get_students()[i]->get_name().c_str()));
        ui->studentListWidget->addItem(item);
    }
    ui->assignmentListWidget->clear();
    for(int j = 0; j < selectedClass->get_assignments().size(); j++) {
        QListWidgetItem *item = new QListWidgetItem;
        item->setText(QString::fromStdString(selectedClass->get_assignments()[j]->get_title().c_str()));
        ui->assignmentListWidget->addItem(item);
    }
}

void BaseScreen::on_addNew_clicked()
{
    QString newClassTitle = ui->classEdit->text();

    if(!newClassTitle.isEmpty()) {
        GAClass *newClass = new GAClass(newClassTitle.toStdString());
        ga->add_class(newClass);
        QListWidgetItem *item = new QListWidgetItem;
        item->setText(newClassTitle);
        ui->classListWidget->addItem(item);
    }
}

//STUDENTS/ASSIGNMENTS PAGE (PAGE 2) SLOTS

void BaseScreen::on_addStudentButton_clicked()
{
    //add dialog
}

void BaseScreen::on_selectStudentButton_clicked()
{
    selectedStudent = selectedClass->get_students()[ui->studentListWidget->currentRow()];
    ui->stackedWidget->setCurrentIndex(3);
}

void BaseScreen::on_addNewAssignmentButton_clicked()
{
   //add dialog
}

void BaseScreen::on_selectAssignmentButton_clicked()
{
    selectedAssignment = selectedClass->get_assignments()[ui->assignmentListWidget->currentRow()];
    ui->stackedWidget->setCurrentIndex(4);
    ui->titleEdit->setText(QString::fromStdString(selectedAssignment->get_title()));
    ui->titleEdit->setReadOnly(true);
    ui->descriptionEdit->clear();
    ui->descriptionEdit->appendPlainText(QString::fromStdString(selectedAssignment->get_description()));
    ui->descriptionEdit->setReadOnly(true);
}

//ASSIGNMENT PAGE (PAGE 4) SLOTS

void BaseScreen::on_editButton_clicked()
{
    ui->titleEdit->setReadOnly(false);
    ui->descriptionEdit->setReadOnly(false);
}

void BaseScreen::on_saveButton_clicked()
{
    selectedAssignment->set_title(ui->titleEdit->text().toStdString());
    ui->titleEdit->setReadOnly(true);
    selectedAssignment->set_description(ui->descriptionEdit->toPlainText().toStdString());
    ui->descriptionEdit->setReadOnly(true);
}
