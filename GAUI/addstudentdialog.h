#ifndef ADDSTUDENTDIALOG_H
#define ADDSTUDENTDIALOG_H

#include <QDialog>
#include <../grading-assistant/gradingassistant.h>
#include <../grading-assistant/gadata/gaclass.h>
#include <../grading-assistant/gadata/gastudent.h>

namespace Ui {
class AddStudentDialog;
}

/*!
 * @brief This is the dialog that the user sees when they indicate that they would like to add
 * a new student.
 */
class AddStudentDialog : public QDialog
{
    Q_OBJECT

public:

    //Constructor/Destructor
    explicit AddStudentDialog(QWidget *parent = 0, GradingAssistant* ga = nullptr);
    ~AddStudentDialog();

    //Get method
    GAStudent* get_new_student();

private slots:

    //Keep track of user actions
    void on_addStudentButton_clicked();

private:
    Ui::AddStudentDialog *ui;

    //Keep track of created data
    GAStudent* newStudent;
    GradingAssistant* gradingAssistant;
};

#endif // ADDSTUDENTDIALOG_H
