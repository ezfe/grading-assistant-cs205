#ifndef FLAGDIALOG_H
#define FLAGDIALOG_H

#include <QDialog>
#include <../grading-assistant/gadata/gaannotation.h>
#include <../grading-assistant/gadata/gaassignmentdata.h>
#include <../grading-assistant/gadata/garubric.h>
#include <../grading-assistant/gadata/garubricrow.h>
#include <../grading-assistant/gradingassistant.h>

namespace Ui {
class FlagDialog;
}

class FlagDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FlagDialog(QWidget *parent = 0, GradingAssistant *g = 0, GARubric *r = 0,
                        std::string type = 0);
    ~FlagDialog();

    void set_top_enabled();
    void set_bottom_enabled();

    GAAnnotation* get_new_annotation();

private slots:
    void on_chooseFromExistingButton_clicked();

    void on_addNewButton_clicked();

    void on_searchBox_returnPressed();

    void on_annotationList_currentRowChanged(int currentRow);

    void on_cancelButton_clicked();

    void on_flagButton_clicked();

private:
    Ui::FlagDialog *ui;

    GradingAssistant *ga;
    GARubric *rubric;

    std::vector<GAAnnotation*> currentAnnotations;
    std::vector<std::string> categories;
    GAAnnotation *selectedAnnotation;
    GAAnnotation *newAnnotation;

    std::string flagType;

};

#endif // FLAGDIALOG_H
