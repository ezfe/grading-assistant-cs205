#ifndef GRADINGDIALOG_H
#define GRADINGDIALOG_H

#include <QDialog>

namespace Ui {
class GradingDialog;
}

class GradingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GradingDialog(QWidget *parent = 0);
    ~GradingDialog();

private:
    Ui::GradingDialog *ui;
};

#endif // GRADINGDIALOG_H
