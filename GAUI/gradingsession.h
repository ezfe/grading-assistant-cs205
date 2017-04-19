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
    void on_studentsToGrade_currentRowChanged(int currentRow);

    void on_flagErrorButton_clicked();

    void on_readyToGradeButton_clicked();

    void on_flagCommentButton_clicked();

    void on_flagECButton_clicked();

    void on_generateOutputButton_clicked();

    void on_searchBox_textChanged(const QString &arg1);

    void on_annotationList_currentRowChanged(int currentRow);

    void print_preview();

    void on_flagButton_clicked();

private:
    Ui::GradingSession *ui;

    GradingAssistant *gradingAssistant;
    GAClass *currentClass;
    GARubric *currentRubric;
    GAAssignment *currentAssignment;
    GAStudent *currentStudent;
    GAAssignmentData *currentAssignmentData;

    std::vector<GAAnnotation*> currentAnnotations;
    GAAnnotation *selectedAnnotation;
    std::string preview;

    FlagDialog *fd;
    GradingDialog *gd;
};

#endif // GRADINGSESSION_H
