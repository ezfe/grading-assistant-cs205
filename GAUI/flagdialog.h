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
    explicit FlagDialog(QWidget *parent = 0, GARubric *r = 0, int i = 0);
    explicit FlagDialog(QWidget *parent = 0, GARubric *r = 0,
                        GAAnnotation* annotation = 0);
    ~FlagDialog();

    GAAnnotation* get_new_annotation();

    //Update view based on user input
    void update_categories();

private slots:

    void on_cancelButton_clicked();

    void on_flagButton_clicked();

    void on_pointsEdit_textChanged(const QString &arg1);

private:

    Ui::FlagDialog *ui;

    //Keeps track of rubric information
    GARubric *rubric;
    std::vector<std::string> categories;

    //Keeps track of annotation information
    GAAnnotation *newAnnotation;
    std::string flagType;

};

#endif // FLAGDIALOG_H
