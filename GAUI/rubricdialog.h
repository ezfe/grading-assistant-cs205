#ifndef RUBRICDIALOG_H
#define RUBRICDIALOG_H

#include <QDialog>

namespace Ui {
class RubricDialog;
}

class RubricDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RubricDialog(QWidget *parent = 0);
    ~RubricDialog();

private:
    Ui::RubricDialog *ui;
};

#endif // RUBRICDIALOG_H
