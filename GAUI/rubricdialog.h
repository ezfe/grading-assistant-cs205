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
    explicit RubricDialog(QWidget *parent = 0, QString t = 0, int r = 0, int c = 0);
    ~RubricDialog();

    GARubric* get_rubric();

private slots:

    //Slots to modify rubric
    void on_addRowButton_clicked();

    void on_deleteRowButton_clicked();

    void on_addColumnButton_clicked();

    void on_deleteColumnButton_clicked();

    void on_rowTitle_clicked();

    void on_columnTitle_clicked();

    //Keeps track of user input
    void on_tableWidget_itemClicked(QTableWidgetItem *item);

    void on_extraCreditButton_stateChanged(int arg1);

    //Close dialog
    void on_cancelButton_clicked();

    void on_saveButton_clicked();

    //Setup GUI
    void setup_table();

    void on_tableWidget_itemChanged(QTableWidgetItem *item);

private:

    Ui::RubricDialog *ui;
    GARubric *myRubric;

    //Keeps track of user input
    QTableWidgetItem *currentItem;

    //Variables associated with rubric being made/edited
    std::string title;
    int rows;
    int cols;

    bool ec;
};

#endif // RUBRICDIALOG_H
