#include "basescreen.h"
#include "ui_basescreen.h"


BaseScreen::BaseScreen(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::BaseScreen)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);
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
    //    for(int i = 0; i < ga.get_classes(); i++) {
    //        ui->classListWidget->addItem(ga.get_classes()[i].get_name());
    //    }
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
    //selectedClass = ga.get_classes()[ui->classListWidget->currentRow()];
    ui->stackedWidget->setCurrentIndex(2);
    ui->studentListWidget->clear();
    //for(int i = 0; i < selectedClass.get_students(); i++) {
    //    ui->studentListWidget->addItem(selectedClass.get_students()[i].get_name());
    //}
    ui->assignmentListWidget->clear();
    //for(int j = 0; j < selectedClass.get_assignments(); j++) {
    //    ui->assignmentListWidget->addItem(selectedClass.get_assignments()[j].get_name());
    //}
}

void BaseScreen::on_addNew_clicked()
{
    QString newClass = ui->classEdit->text();

    if(!newClass.isEmpty()) {
        //ga.add_class(new GAClass(newClass.toStdString()))
        //ui->classListWidget->addItem(ga.get_classes().back().get_title());
    }
}

//STUDENTS/ASSIGNMENTS PAGE (PAGE 2) SLOTS

void BaseScreen::on_addStudentButton_clicked()
{
    //add dialog
}

void BaseScreen::on_selectStudentButton_clicked()
{
    //selectedStudent = selectedClass.get_classes()[ui->studentListWidget->currentRow()];
    ui->stackedWidget->setCurrentIndex(3);
}

void BaseScreen::on_addNewAssignmentButton_clicked()
{
   //add dialog
}

void BaseScreen::on_selectAssignmentButton_clicked()
{
    //selectedAssignment = selectedAssignment.get_assignments()[ui->assignmentListWidget->currentRow()];
    ui->stackedWidget->setCurrentIndex(4);
    //ui->titleEdit->setText(selectedAssignment->get__title());
    ui->titleEdit->setReadOnly(true);
    //ui->descriptionEdit->setText(selectedAssignment->get_dscription());
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
    //selectedAssignment->set_title(ui->titleEdit->text());
    ui->titleEdit->setReadOnly(true);
    //selectedAssignment->set_description(ui->descriptionEdit->text());
    ui->descriptionEdit->setReadOnly(true);
}
