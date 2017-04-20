#ifndef GRADINGDIALOG_H
#define GRADINGDIALOG_H

#include <QDialog>
#include <../grading-assistant/gadata/gaassignmentdata.h>
#include <../grading-assistant/gadata/gaannotation.h>
#include <../grading-assistant/gadata/garubric.h>
#include <../grading-assistant/gadata/gastudent.h>
#include <QLineEdit>
#include <QInputDialog>

namespace Ui {
class GradingDialog;
}

class GradingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GradingDialog(QWidget *parent = 0, GAStudent *s = 0, GARubric *r = 0,
                           GAAssignmentData *d = 0);
    ~GradingDialog();

    //Setup view
    void setup_dialog();
    void setup_annotations();
    void setup_table();

private slots:
    void on_rubricWidget_cellDoubleClicked(int row, int column);

    void on_saveButton_clicked();

private:
    Ui::GradingDialog *ui;

    //Keep track of presented data
    GAStudent *student;
    GARubric *rubric;
    GAAssignmentData *data;

    //General variables to keep track of commonly used values
    int rows;
    int cols;
};

#endif // GRADINGDIALOG_H
