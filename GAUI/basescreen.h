#ifndef BASESCREEN_H
#define BASESCREEN_H

#include <QMainWindow>
#include <QFileDialog>
#include <QListWidgetItem>
#include "../grading-assistant/gradingassistant.h"
#include "../grading-assistant/gadata/gaclass.h"
#include "../grading-assistant/gadata/gastudent.h"
#include "../grading-assistant/gadata/gaassignment.h"
#include "../grading-assistant/gadata/garubric.h"
#include "../grading-assistant/gadata/garubricrow.h"
#include "../grading-assistant/filemanager.h"
#include "rubricdialog.h"
#include "setupsessiondialog.h"
#include "gradingsession.h"
#include "addassignmentdialog.h"
#include "addstudentdialog.h"

class RubricDialog;
class AddAssignmentDialog;
class SetupSessionDialog;

namespace Ui {
class BaseScreen;
}

class BaseScreen : public QMainWindow
{
    Q_OBJECT

public:
    explicit BaseScreen(QWidget *parent = 0);
    ~BaseScreen();

    void start_grading(GAClass *c, GARubric *r, GAAssignment *a);

private slots:

    //WELCOME PAGE (PAGE 0) SLOTS
    void on_actionBack_triggered();

    void on_actionClasses_triggered();

    void on_actionRubrics_triggered();

    void on_actionCurrent_Session_triggered();

    void on_actionQuit_triggered();

    void on_selectFilePathButton_clicked();

    void on_importButton_clicked();

    //CLASS PAGE (PAGE 1) SLOTS
    void on_deleteButton_clicked();

    void on_addNew_clicked();

    //STUDENTS/ASSIGNMENTS (PAGE 2) SLOTS
    void on_addStudentButton_clicked();

    void on_addNewAssignmentButton_clicked();

    //ASSIGNMENT (PAGE 5) SLOTS
    void on_editButton_clicked();

    void on_saveButton_clicked();

    //RUBRIC (PAGE 4) SLOTS

    void on_studentListWidget_itemDoubleClicked(QListWidgetItem *item);

    void on_assignmentListWidget_itemDoubleClicked(QListWidgetItem *item);

    void on_classListWidget_itemDoubleClicked(QListWidgetItem *item);

    void on_rubricListWidget_itemDoubleClicked(QListWidgetItem *item);

    void on_temp_save_clicked();

private:
    Ui::BaseScreen *ui;

    //Dialogs
    RubricDialog *rd;
    SetupSessionDialog *ssd;
    GradingSession *gs;
    AddAssignmentDialog *aad;
    AddStudentDialog *asd;

    GradingAssistant *ga;

    //Currently selected objects
    GAClass *selectedClass;
    GAStudent *selectedStudent;
    GAAssignment *selectedAssignment;
    GARubric *selectedRubric;
};

#endif // BASESCREEN_H
