#ifndef ADDASSIGNMENTDIALOG_H
#define ADDASSIGNMENTDIALOG_H

#include <../grading-assistant/gadata/gaassignment.h>
#include <../grading-assistant/gradingassistant.h>
#include <../grading-assistant/gadata/gaclass.h>
#include <QDialog>

namespace Ui {
class AddAssignmentDialog;
}

class AddAssignmentDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddAssignmentDialog(QWidget *parent = 0);
    ~AddAssignmentDialog();

private:
    Ui::AddAssignmentDialog *ui;
};

#endif // ADDASSIGNMENTDIALOG_H
