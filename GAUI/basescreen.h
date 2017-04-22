#ifndef BASESCREEN_H
#define BASESCREEN_H

#include <QMainWindow>
#include <QFileDialog>
#include <QListWidgetItem>
#include <QShortcut>
#include <QMenu>
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

//Forward Declarations
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

    void setup_general();

    //void start_grading(GAClass *c, GARubric *r, GAAssignment *a);

private slots:

    //WELCOME PAGE (PAGE 0) SLOTS
    void on_actionBack_triggered();

    void on_actionClasses_triggered();

    void on_actionRubrics_triggered();

    void on_actionCurrent_Session_triggered();

    void on_gs_close();

    void on_actionQuit_triggered();

    void on_importButton_clicked();

    void on_temp_save_clicked();

    //CLASS PAGE (PAGE 1) SLOTS
    void on_deleteButton_clicked();

    void on_addNew_clicked();

    void on_classListWidget_itemDoubleClicked(QListWidgetItem *item);

    //STUDENTS/ASSIGNMENTS (PAGE 2) SLOTS
    void on_addStudentButton_clicked();

    void on_addNewAssignmentButton_clicked();

    void on_studentListWidget_itemDoubleClicked(QListWidgetItem *item);

    void on_assignmentListWidget_itemDoubleClicked(QListWidgetItem *item);

    void show_context_menu(const QPoint &pos);

    void show_context_menu2(const QPoint &pos);

    void delete_student();

    void delete_assignment();

    //ASSIGNMENT (PAGE 5) SLOTS
    void on_saveButton_clicked();

    void on_editRubricButton_clicked();

    //RUBRIC (PAGE 4) SLOTS
    void on_rubricListWidget_itemDoubleClicked(QListWidgetItem *item);

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
