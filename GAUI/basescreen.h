#ifndef BASESCREEN_H
#define BASESCREEN_H

#include <QMainWindow>
#include <QFileDialog>
#include "../grading-assistant/gradingassistant.h"
#include "../grading-assistant/gadata/gaclass.h"
#include "../grading-assistant/gadata/gastudent.h"
#include "../grading-assistant/gadata/gaassignment.h"
#include "../grading-assistant/filemanager.h"

namespace Ui {
class BaseScreen;
}

class BaseScreen : public QMainWindow
{
    Q_OBJECT

public:
    explicit BaseScreen(QWidget *parent = 0);
    ~BaseScreen();

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

    void on_selectButton_clicked();

    void on_addNew_clicked();

    //STUDENTS/ASSIGNMENTS (PAGE 2) SLOTS
    void on_addStudentButton_clicked();

    void on_selectStudentButton_clicked();

    void on_addNewAssignmentButton_clicked();

    void on_selectAssignmentButton_clicked();

    //ASSIGNMENT (PAGE 4) SLOTS
    void on_editButton_clicked();

    void on_saveButton_clicked();

private:
    Ui::BaseScreen *ui;
    GradingAssistant *ga;
    GAClass *selectedClass;
    GAStudent *selectedStudent;
    GAAssignment *selectedAssignment;
};

#endif // BASESCREEN_H
