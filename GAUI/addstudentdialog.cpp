#include "addstudentdialog.h"
#include "ui_addstudentdialog.h"

/*!
 * @brief Creates a dialog that allows the user to create a new student
 * @param parent - BaseScreen
 */
AddStudentDialog::AddStudentDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddStudentDialog)
{
    ui->setupUi(this);
    newStudent = nullptr;
}


/*!
 * @brief Destructs everything created for this dialog
 */
AddStudentDialog::~AddStudentDialog()
{
    delete ui;
}


/*!
 * @brief Gets the newly created student object
 * @return newStudent
 */
GAStudent* AddStudentDialog::get_new_student() {
    return newStudent;
}


/*!
 * @brief Slot called when user clicks the Add button
 */
void AddStudentDialog::on_addStudentButton_clicked()
{
    //if the user has not filled out all the edits, do nothing
    if(ui->nameEdit->text().isEmpty() || ui->usernameEdit->text().isEmpty()) {
        return;
    }
    //create new student
    newStudent = new GAStudent(ui->nameEdit->text().toStdString(),
                               ui->usernameEdit->text().toStdString());
    close();
}
