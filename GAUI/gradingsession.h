#ifndef GRADINGSESSION_H
#define GRADINGSESSION_H

#include <QDialog>
#include <../grading-assistant/gradingassistant.h>
#include <../grading-assistant/gadata/gaclass.h>
#include <../grading-assistant/gadata/garubric.h>
#include <../grading-assistant/gadata/gaassignment.h>
#include <../grading-assistant/gadata/gaassignmentdata.h>
#include <flagdialog.h>
#include <gradingdialog.h>

namespace Ui {
class GradingSession;
}

class GradingSession : public QDialog
{
    Q_OBJECT

public:
    explicit GradingSession(QWidget *parent = 0, GradingAssistant *ga = 0,
                            GAClass *c = 0, GARubric *r = 0, GAAssignment *a = 0);
    ~GradingSession();

    void setup_dialog();

private slots:
    void on_studentsToGrade_currentRowChanged(int currentRow);

    void on_flagTest_clicked();

    void on_readyToGradeButton_clicked();

private:
    Ui::GradingSession *ui;

    GradingAssistant *gradingAssistant;
    GAClass *currentClass;
    GARubric *currentRubric;
    GAAssignment *currentAssignment;
    GAStudent *currentStudent;
    GAAssignmentData *currentAssignmentData;

    FlagDialog *fd;
    GradingDialog *gd;
};

#endif // GRADINGSESSION_H
