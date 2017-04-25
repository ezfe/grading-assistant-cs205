#ifndef ADDASSIGNMENTDIALOG_H
#define ADDASSIGNMENTDIALOG_H

#include <../grading-assistant/gadata/gaassignment.h>
#include <../grading-assistant/gradingassistant.h>
#include <../grading-assistant/gadata/gaclass.h>
#include <QDialog>
#include <rubricdialog.h>

class RubricDialog;

namespace Ui {
class AddAssignmentDialog;
}

/*!
 * @brief This is the dialog that the user sees when they indicate that they would like to create
 * a new assignmnet. It allows the user to input information about the assignment and pick a
 * rubric to use for the assignment.
 */
class AddAssignmentDialog : public QDialog
{
    Q_OBJECT

public:

    //Constructor/Destructor
    explicit AddAssignmentDialog(QWidget *parent = 0, GradingAssistant *g = 0, GAClass* c = 0);
    ~AddAssignmentDialog();

    //Get method
    GAAssignment* get_new_assignment();

private slots:

    //Keep track of user actions
    void on_nextButton_clicked();

    void on_selectExistingButton_clicked();

    void on_addNewButton_clicked();

private:
    Ui::AddAssignmentDialog *ui;

    //Possible child dialog
    RubricDialog * rd;

    //Keep track of selected data
    GradingAssistant* ga;
    GAAssignment* newAssignment;
    GAClass* currentClass;
};

#endif // ADDASSIGNMENTDIALOG_H
