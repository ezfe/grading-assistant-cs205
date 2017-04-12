#ifndef GRADINGDIALOG_H
#define GRADINGDIALOG_H

#include <QDialog>
#include <../grading-assistant/gadata/gaassignmentdata.h>
#include <../grading-assistant/gadata/gaannotation.h>
#include <../grading-assistant/gadata/garubric.h>
#include <../grading-assistant/gadata/gastudent.h>
#include <QLineEdit>

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

    void setup_dialog();
    void setup_table();
    void setup_annotations();

private:
    Ui::GradingDialog *ui;

    GAStudent *student;
    GARubric *rubric;
    GAAssignmentData *data;

    int rows;
    int cols;
    int score;
};

#endif // GRADINGDIALOG_H
