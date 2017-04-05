#include "rubricdialog.h"
#include "ui_rubricdialog.h"

RubricDialog::RubricDialog(QWidget *parent, GARubric *g) :
    QDialog(parent),
    ui(new Ui::RubricDialog)
{
    ui->setupUi(this);
}

RubricDialog::RubricDialog(QWidget *parent, QString t, int r, int c, int p) :
    QDialog(parent),
    ui(new Ui::RubricDialog)
{
    ui->setupUi(this);
    this->setWindowTitle(t);

    title = t.toStdString();
    rows = r;
    cols = c;
    maxPoints = p;

    ui->tableWidget->setRowCount(rows + 1);
    ui->tableWidget->setColumnCount(cols + 1);

    //set up column headers
    for(int k = 0; k < (cols + 1); k++)
    {
        QTableWidgetItem *item = new QTableWidgetItem(2);
        if(k != cols) {
            QTableWidgetItem *item = new QTableWidgetItem(2);
            item->setFlags(Qt::ItemIsEditable);
            item->setText("Column " + QString::number(k + 1));
            ui->tableWidget->setHorizontalHeaderItem(k, item);
        }
        else {
            item->setText("Out Of: ");
            item->setFlags(!Qt::ItemIsEditable);
            ui->tableWidget->setHorizontalHeaderItem(cols, item);
        }
    }

    //fill grid with items
    for(int i = 0; i < rows; i++) {

        //set up row headers
        QTableWidgetItem *item = new QTableWidgetItem(2);
        item->setText("Category " + QString::number(i + 1));
        item->setFlags(Qt::ItemIsEditable);
        ui->tableWidget->setVerticalHeaderItem(i, item);

        for(int j = 0; j < cols; j++) {
            if(j != (cols - 1)) {
                QTableWidgetItem *item = new QTableWidgetItem(2);
                item->setFlags(Qt::ItemIsEditable);
                ui->tableWidget->setItem(i, j, item);
            }
            else {
                QTableWidgetItem *item = new QTableWidgetItem(1);
                item->setFlags(Qt::ItemIsEditable);
                ui->tableWidget->setItem(i, j, item);
            }
        }
    }

    //set up last row of table
    QTableWidgetItem *pointCategory = new QTableWidgetItem(2);
    pointCategory->setText("Total Points: ");
    pointCategory->setFlags(!Qt::ItemIsEditable);
    ui->tableWidget->setVerticalHeaderItem(rows, pointCategory);

    //make last row uneditable
    for(int m = 0; m < cols; m++)
    {
        QTableWidgetItem *item = new QTableWidgetItem(2);
        item->setFlags(!Qt::ItemIsEditable);
        ui->tableWidget->setItem(rows, m, item);
    }

    //last item is total point value
    QTableWidgetItem *pointValue = new QTableWidgetItem(1);
    pointValue->setText(QString::number(maxPoints));
    pointValue->setFlags(!Qt::ItemIsEditable);
    ui->tableWidget->setItem(rows, cols, pointValue);

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
    ui->tableWidget->item(rows, cols);
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

void RubricDialog::on_tableWidget_itemDoubleClicked(QTableWidgetItem *item)
{

}
