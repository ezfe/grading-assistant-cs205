#include "addstudentdialog.h"
#include "ui_addstudentdialog.h"


//CONSTRUCTORS/DESTRUCTORS


/*!
 * @brief Creates a dialog that allows the user to create a new student
 * @param parent - BaseScreen
 * @param ga - Grading Assistant
 */
AddStudentDialog::AddStudentDialog(QWidget *parent, GradingAssistant* ga) :
    QDialog(parent),
    ui(new Ui::AddStudentDialog)
{
    ui->setupUi(this);
    newStudent = nullptr;
    gradingAssistant = ga;

    this->setWindowTitle("Add Student");
}


/*!
 * @brief Destructs everything created for this dialog
 */
AddStudentDialog::~AddStudentDialog()
{
    delete ui;
}


//GET METHOD


/*!
 * @brief Gets the newly created student object
 * @return newStudent
 */
GAStudent* AddStudentDialog::get_new_student() {
    return newStudent;
}


//KEEP TRACK OF USER ACTIONS


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
    newStudent = new GAStudent(this->gradingAssistant);
    newStudent->set_name(ui->nameEdit->text().toStdString());
    newStudent->set_lafayette_username(ui->usernameEdit->text().toStdString());

    close();
}
