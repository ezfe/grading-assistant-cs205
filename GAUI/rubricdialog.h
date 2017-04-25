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

/*!
 * @brief This class is the dialog that allows users to create new or edit existing rubrics
 */
class RubricDialog : public QDialog
{
    Q_OBJECT

public:

    //Constructors/Destructor
    explicit RubricDialog(QWidget *parent = 0, GARubric *g = 0, GradingAssistant* ga = 0);
    explicit RubricDialog(QWidget *parent = 0, int r = 0, int c = 0, GradingAssistant* ga = 0,
                          GAClass* cc = 0, GAAssignment* a = 0);
    ~RubricDialog();

    //Get method
    GARubric* get_rubric();

private slots:

    //Setup GUI
    void setup_table();

    //Slots to modify rubric
    void on_addRowButton_clicked();

    void on_deleteRowButton_clicked();

    void on_addColumnButton_clicked();

    void on_deleteColumnButton_clicked();

    //Keeps track of user input
    void on_tableWidget_itemClicked(QTableWidgetItem *item);

    void on_extraCreditButton_stateChanged(int arg1);

    void on_tableWidget_itemChanged(QTableWidgetItem *item);

    //Close dialog
    void on_cancelButton_clicked();

    void on_saveButton_clicked();

private:

    Ui::RubricDialog *ui;
    GARubric *myRubric;
    GradingAssistant* grading_assistant;

    //Keeps track of user input
    QTableWidgetItem *currentItem;

    //Variables associated with rubric being made/edited
    std::string title;
    int rows;
    int cols;

    bool ec;
};

#endif // RUBRICDIALOG_H
