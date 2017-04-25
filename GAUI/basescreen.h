#ifndef BASESCREEN_H
#define BASESCREEN_H

#include <QMainWindow>
#include <QFileDialog>
#include <QListWidgetItem>
#include <QShortcut>
#include <QMenu>
#include <ctime>
#include <QTime>

#include "../grading-assistant/gradingassistant.h"
#include "../grading-assistant/gadata/gaclass.h"
#include "../grading-assistant/gadata/gastudent.h"
#include "../grading-assistant/gadata/gaassignment.h"
#include "../grading-assistant/gadata/garubric.h"
#include "../grading-assistant/gadata/garubricrow.h"
#include "../grading-assistant/filemanager.h"
#include "../grading-assistant/usersettings.h"

#include "../server/githandler.h"

#include "rubricdialog.h"
#include "setupsessiondialog.h"
#include "gradingsession.h"
#include "addassignmentdialog.h"
#include "addstudentdialog.h"
#include "configuresettings.h"
#include "gradingdialog.h"

//Forward Declarations
class RubricDialog;
class AddAssignmentDialog;
class SetupSessionDialog;

namespace Ui {
class BaseScreen;
}

/*!
 * @brief The BaseScreen class is our program's MainWindow; all further dialogs/actions start
 * from one of this screen's pages.
 */
class BaseScreen : public QMainWindow
{
    Q_OBJECT

public:

    //Constructor/Destructor
    explicit BaseScreen(QWidget *parent = 0);
    ~BaseScreen();

    //Miscellaneous methods
    void setup_general();

    std::string get_time();

    void delete_if_needed();

private slots:

    //WELCOME PAGE (PAGE 0) SLOTS
    void on_actionBack_triggered();

    void on_actionClasses_triggered();

    void on_actionRubrics_triggered();

    void on_actionCurrent_Session_triggered();

    void on_actionImport_triggered();

    void on_actionSave_triggered();

    void on_actionQuit_triggered();

    void on_importButton_clicked();

    //CLASS PAGE (PAGE 1) SLOTS / METHODS
    void on_classListWidget_itemDoubleClicked(QListWidgetItem *item);

    void show_context_menu_class(const QPoint &pos);

    void delete_class();

    void on_addNew_clicked();

    void update_class_list();

    void update_year_box();

    void on_pickSemesterBox_currentIndexChanged(int index);

    void on_pickYearBox_currentIndexChanged(int index);

    //STUDENTS/ASSIGNMENTS (PAGE 2) SLOTS / METHODS
    void on_studentListWidget_itemDoubleClicked(QListWidgetItem *item);

    void on_addStudentButton_clicked();

    void on_assignmentListWidget_itemDoubleClicked(QListWidgetItem *item);

    void delete_assignment_table();

    void on_addNewAssignmentButton_clicked();

    void show_context_menu_students(const QPoint &pos);

    void show_context_menu_assignments(const QPoint &pos);

    void delete_student();

    void delete_assignment();

    void on_gradebookButton_clicked();

    void delete_gradebook_table();

    //STUDENT (PAGE 3) SLOTS
    void on_editStudentButton_clicked();

    void on_saveStudentButton_clicked();

    void on_pastAssignmentsWidget_itemDoubleClicked(QListWidgetItem *item);

    //ASSIGNMENT (PAGE 5) SLOTS
    void on_saveButton_clicked();

    void on_editRubricButton_clicked();

    //RUBRIC (PAGE 4) SLOTS
    void on_rubricListWidget_itemDoubleClicked(QListWidgetItem *item);

private:
    Ui::BaseScreen *ui;

    //Dialogs
    ConfigureSettings *cs;
    RubricDialog *rd;
    SetupSessionDialog *ssd;
    GradingSession *gs;
    AddAssignmentDialog *aad;
    AddStudentDialog *asd;
    GradingDialog *gd;

    GradingAssistant *ga;

    GitHandler* serverHandler = nullptr;

    //Currently selected objects
    std::vector<GAClass*> currentClassList;
    GAClass *selectedClass;
    GAStudent *selectedStudent;
    GAAssignment *selectedAssignment;
    GARubric *selectedRubric;
    std::vector<GAAssignmentData*> currentList;

    //Aids in memory management
    bool deleteTable;
    bool deleteGradebook;

    void sync_remote();
};

#endif // BASESCREEN_H
