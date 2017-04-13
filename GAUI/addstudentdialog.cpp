#include "addstudentdialog.h"
#include "ui_addstudentdialog.h"

AddStudentDialog::AddStudentDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddStudentDialog)
{
    ui->setupUi(this);
    newStudent = nullptr;
}

AddStudentDialog::~AddStudentDialog()
{
    delete ui;
}

void AddStudentDialog::on_addStudentButton_clicked()
{
    newStudent = new GAStudent(ui->nameEdit->text().toStdString(),
                               ui->usernameEdit->text().toStdString());
    close();
}

GAStudent* AddStudentDialog::get_new_student() {
    return newStudent;
}
