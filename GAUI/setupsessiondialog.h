#ifndef SETUPSESSIONDIALOG_H
#define SETUPSESSIONDIALOG_H

#include <QDialog>
#include <../grading-assistant/gradingassistant.h>
#include <../grading-assistant/gadata/gaclass.h>
#include <../grading-assistant/gadata/garubric.h>
#include <../grading-assistant/gadata/gaassignment.h>
#include <addassignmentdialog.h>

class AddAssignmentDialog;

namespace Ui {
class SetupSessionDialog;
}

/*!
 * @brief This class is the dialog that allows the user to pick which class and assignment they
 * would like to grade in their grading session
 */
class SetupSessionDialog : public QDialog
{
    Q_OBJECT

public:

    //Constructor/Destructor
    explicit SetupSessionDialog(QWidget *parent = 0, GradingAssistant *ga = 0);
    ~SetupSessionDialog();

    //Get methods
    GAClass* get_selected_class();

    GAAssignment* get_selected_assignment();

private slots:

    //Keep track of user actions
    void on_openSessionButton_clicked();

    void on_classComboBox_currentIndexChanged(int index);

private:
    Ui::SetupSessionDialog *ui;

    GradingAssistant *ga;

    //Possible child dialog
    AddAssignmentDialog *aad;

    //Keeps track of selections
    GAClass* selectedClass;
    GAAssignment* selectedAssignment;
};

#endif // SETUPSESSIONDIALOG_H
