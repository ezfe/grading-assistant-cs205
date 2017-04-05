#include "rubricdialog.h"
#include "ui_rubricdialog.h"

RubricDialog::RubricDialog(QWidget *parent, GARubric *g) :
    QDialog(parent),
    ui(new Ui::RubricDialog)
{
    ui->setupUi(this);
}

RubricDialog::RubricDialog(QWidget *parent, std::string t, int r, int c, int p) :
    QDialog(parent),
    ui(new Ui::RubricDialog)
{
    ui->setupUi(this);
    this->setWindowTitle(QString::fromStdString(t.c_str()));

    rows = r + 1; //rows + total row
    cols = c + 1; //cols + point col
    maxPoints = p;

    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < cols; j++) {

        }
    }

}

RubricDialog::~RubricDialog()
{
    delete ui;
}

void RubricDialog::on_buttonBox_accepted()
{

}

void RubricDialog::on_buttonBox_rejected()
{

}

void RubricDialog::on_pointValueButton_clicked()
{

}

void RubricDialog::on_addRowButton_clicked()
{

}

void RubricDialog::on_deleteRowButton_clicked()
{

}

void RubricDialog::on_addColumnButton_clicked()
{

}

void RubricDialog::on_deleteColumnButton_clicked()
{

}

void RubricDialog::on_tableWidget_cellClicked(int row, int column)
{

}

void RubricDialog::on_tableWidget_itemClicked(QTableWidgetItem *item)
{

}
