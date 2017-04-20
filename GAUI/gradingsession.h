#ifndef GRADINGSESSION_H
#define GRADINGSESSION_H

#include <QDialog>
#include <../grading-assistant/gradingassistant.h>
#include <../grading-assistant/gadata/gaclass.h>
#include <../grading-assistant/gadata/garubric.h>
#include <../grading-assistant/gadata/gaassignment.h>
#include <../grading-assistant/gadata/gaassignmentdata.h>
#include <../grading-assistant/gadata/gaannotation.h>
#include <../grading-assistant/gaoutputfile.h>
#include <../grading-assistant/filemanager.h>
#include <flagdialog.h>
#include <gradingdialog.h>
#include <codetextedit.h>
#include <QFileDialog>
#include <QTabWidget>

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

    //Slots to keep track of list widget changes
    void on_studentsToGrade_currentRowChanged(int currentRow);

    void on_fileList_currentRowChanged(int currentRow);

    void on_annotationList_currentRowChanged(int currentRow);

    //Slots to keep track of button clicks
    void on_readyToGradeButton_clicked();

    void on_generateOutputButton_clicked();

    void on_flagButton_clicked();

    void on_editButton_clicked();

    void on_addNewButton_clicked();

    //Slots to keep track of annotation changes
    void on_searchBox_textChanged(const QString &arg1);

    void print_preview();

    void update_selection();

private:
    Ui::GradingSession *ui;

    //Keep track of current/selected variables and data
    GradingAssistant *gradingAssistant;
    GAClass *currentClass;
    GARubric *currentRubric;
    GAAssignment *currentAssignment;
    GAStudent *currentStudent;
    GAAssignmentData *currentAssignmentData;

    //Keep track of current files
    std::vector<std::pair<std::string, std::string>> studentFiles;
    std::string currentFile;

    //Keep track of current annotations
    std::vector<GAAnnotation*> currentAnnotations;
    GAAnnotation *selectedAnnotation;
    std::string preview;

    //Dialogs that this widget creates
    FlagDialog *fd;
    GradingDialog *gd;
};

#endif // GRADINGSESSION_H
