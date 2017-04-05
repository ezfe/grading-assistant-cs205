#ifndef RUBRICDIALOG_H
#define RUBRICDIALOG_H

#include <QDialog>
#include "basescreen.h"
#include <QTableWidgetItem>

namespace Ui {
class RubricDialog;
}

class RubricDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RubricDialog(QWidget *parent = 0, GARubric *g = 0);
    explicit RubricDialog(QWidget *parent = 0, QString t = 0, int r = 0, int c = 0,
                          int p = 0);
    ~RubricDialog();

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

    void on_pointValueButton_clicked();

    void on_addRowButton_clicked();

    void on_deleteRowButton_clicked();

    void on_addColumnButton_clicked();

    void on_deleteColumnButton_clicked();

    void on_tableWidget_cellClicked(int row, int column);

    void on_tableWidget_itemClicked(QTableWidgetItem *item);

    void on_tableWidget_itemDoubleClicked(QTableWidgetItem *item);

private:
    Ui::RubricDialog *ui;
    GARubric *myRubric;

    std::string title;
    int rows;
    int cols;
    int maxPoints;
};

#endif // RUBRICDIALOG_H
