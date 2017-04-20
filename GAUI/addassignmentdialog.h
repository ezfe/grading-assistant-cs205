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

class AddAssignmentDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddAssignmentDialog(QWidget *parent = 0, GradingAssistant *g = 0);
    ~AddAssignmentDialog();

    GAAssignment* get_new_assignment();

private slots:
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
};

#endif // ADDASSIGNMENTDIALOG_H
