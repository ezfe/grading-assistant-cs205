#ifndef ADDSTUDENTDIALOG_H
#define ADDSTUDENTDIALOG_H

#include <QDialog>
#include <../grading-assistant/gradingassistant.h>
#include <../grading-assistant/gadata/gaclass.h>
#include <../grading-assistant/gadata/gastudent.h>

namespace Ui {
class AddStudentDialog;
}

class AddStudentDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddStudentDialog(QWidget *parent = 0);
    ~AddStudentDialog();

    GAStudent* get_new_student();

private slots:
    void on_addStudentButton_clicked();

private:
    Ui::AddStudentDialog *ui;

    GAStudent *newStudent;
};

#endif // ADDSTUDENTDIALOG_H
