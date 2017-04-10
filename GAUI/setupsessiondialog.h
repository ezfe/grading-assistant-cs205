#ifndef SETUPSESSIONDIALOG_H
#define SETUPSESSIONDIALOG_H

#include <QDialog>
#include <../grading-assistant/gradingassistant.h>
#include <../grading-assistant/gadata/gaclass.h>
#include <../grading-assistant/gadata/garubric.h>
#include <../grading-assistant/gadata/gaassignment.h>

namespace Ui {
class SetupSessionDialog;
}

class SetupSessionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SetupSessionDialog(QWidget *parent = 0, GradingAssistant *ga = 0);
    ~SetupSessionDialog();

    GAClass* get_selected_class();

    GARubric* get_selected_rubric();

    GAAssignment* get_selected_assignment();

private slots:
    void on_pushButton_clicked();

    void on_classComboBox_currentIndexChanged(int index);

private:
    Ui::SetupSessionDialog *ui;

    GradingAssistant *ga;

    GAClass* selectedClass;
    GARubric* selectedRubric;
    GAAssignment* selectedAssignment;
};

#endif // SETUPSESSIONDIALOG_H