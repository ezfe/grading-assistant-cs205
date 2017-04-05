#ifndef RUBRICDIALOG_H
#define RUBRICDIALOG_H

#include <QDialog>
#include "basescreen.h"
#include <QTableWidgetItem>
#include <QInputDialog>
#include <QMessageBox>

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

    void on_addRowButton_clicked();

    void on_deleteRowButton_clicked();

    void on_addColumnButton_clicked();

    void on_deleteColumnButton_clicked();

    void on_tableWidget_itemClicked(QTableWidgetItem *item);

    void on_rowTitle_clicked();

    void on_columnTitle_clicked();

    void on_cancelButton_clicked();

    void on_saveButton_clicked();

    void setup_table();

private:
    Ui::RubricDialog *ui;
    GARubric *myRubric;

    QTableWidgetItem *currentItem;

    std::string title;
    int maxPoints;
    int rows;
    int cols;
};

#endif // RUBRICDIALOG_H
