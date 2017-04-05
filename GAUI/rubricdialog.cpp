#include "rubricdialog.h"
#include "ui_rubricdialog.h"

RubricDialog::RubricDialog(QWidget *parent, GARubric *g) :
    QDialog(parent),
    ui(new Ui::RubricDialog)
{
    ui->setupUi(this);
    this->setWindowTitle(QString::fromStdString(g->get_title()));

    title = g->get_title();
    rows = g->get_rows().size();
    cols = g->get_rows()[0]->get_descriptions().size();
    maxPoints = g->get_max_points();

    currentItem = nullptr;
    myRubric = g;

    setup_table();
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

    currentItem = nullptr;
    myRubric = nullptr;

    setup_table();
}

void RubricDialog::setup_table()
{
    ui->tableWidget->setRowCount(rows + 1);
    ui->tableWidget->setColumnCount(cols + 1);

    //set up column headers
    for(int k = 0; k < (cols + 1); k++)
    {
        QTableWidgetItem *item = new QTableWidgetItem(2);
        if(k != cols) {
            QTableWidgetItem *item = new QTableWidgetItem(2);
            item->setText("Column");
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
        if(myRubric == nullptr) {
            item->setText("Category");
        }
        else {
            item->setText(QString::fromStdString(myRubric->get_rows()[i]->get_category()));
        }
        ui->tableWidget->setVerticalHeaderItem(i, item);

        for(int j = 0; j < cols+1; j++) {
            if(j != cols) {
                QTableWidgetItem *item = new QTableWidgetItem(2);
                if(myRubric != nullptr) {
                    item->setText(QString::fromStdString(myRubric->get_rows()[i]->get_descriptions()[j]));
                }
                ui->tableWidget->setItem(i, j, item);
            }
            else {
                QTableWidgetItem *item = new QTableWidgetItem(1);
                if(myRubric != nullptr) {
                    item->setText(QString::number(myRubric->get_rows()[i]->get_max_points()));
                }
                ui->tableWidget->setItem(i, j, item);
            }
        }
    }

    //set up last row of table
    QTableWidgetItem *pointCategory = new QTableWidgetItem(2);
    pointCategory->setText("Total Points: ");
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
    ui->tableWidget->setItem(rows, cols, pointValue);

}

RubricDialog::~RubricDialog()
{
    for(int i = 0; i < (rows+1); i++)
    {
        if(ui->tableWidget->verticalHeaderItem(i) != nullptr) {
            delete ui->tableWidget->verticalHeaderItem(i);
        }
        for(int j = 0; j < (cols+1); j++)
        {
            if(ui->tableWidget->horizontalHeaderItem(j) != nullptr) {
                delete ui->tableWidget->horizontalHeaderItem(j);
            }
            if(ui->tableWidget->item(i, j) != nullptr) {
                delete ui->tableWidget->item(i,j);
            }
        }
    }

    delete ui;
}

void RubricDialog::on_addRowButton_clicked()
{
    ui->tableWidget->insertRow(rows);
    QTableWidgetItem *item = new QTableWidgetItem(2);
    item->setText("Category");
    ui->tableWidget->setVerticalHeaderItem(rows, item);

    for(int i = 0; i < cols; i++) {
        QTableWidgetItem *item = new QTableWidgetItem(2);
        ui->tableWidget->setItem(rows, i, item);
    }

    QTableWidgetItem *num = new QTableWidgetItem(1);
    ui->tableWidget->setItem(rows, cols, num);

    rows++;
}

void RubricDialog::on_deleteRowButton_clicked()
{
    //don't delete last row
    if(currentItem == nullptr) {
        return;
    }
    else if(currentItem->row() == (rows)) {
        return;
    }
    else
    {
        ui->tableWidget->removeRow(currentItem->row());
        rows--;
        currentItem = nullptr;
    }
}

void RubricDialog::on_addColumnButton_clicked()
{
    ui->tableWidget->insertColumn(cols-1);
    QTableWidgetItem *item = new QTableWidgetItem(2);
    item->setText("Column");
    ui->tableWidget->setHorizontalHeaderItem(cols-1, item);

    for(int i = 0; i < rows; i++) {
        QTableWidgetItem *item = new QTableWidgetItem(2);
        ui->tableWidget->setItem(i, cols-1, item);
    }

    QTableWidgetItem *blank = new QTableWidgetItem(2);
    blank->setFlags(!Qt::ItemIsEditable);
    ui->tableWidget->setItem(rows, cols-1, blank);

    cols++;
}

void RubricDialog::on_deleteColumnButton_clicked()
{
    //don't delete last column
    if(currentItem == nullptr) {
        return;
    }
    else if(currentItem->column() == (cols)) {
        return;
    }
    else
    {
        ui->tableWidget->removeColumn(currentItem->column());
        cols--;
        currentItem = nullptr;
    }
}

void RubricDialog::on_tableWidget_itemClicked(QTableWidgetItem *item)
{
    currentItem = item;
}

void RubricDialog::on_rowTitle_clicked()
{
    if(currentItem == nullptr) {
        return;
    }
    else if(currentItem->row() == rows) {
        return;
    }
    QString category = QInputDialog::getText(this, "Set Category",
                                             "Enter Category Title: ");
    ui->tableWidget->verticalHeaderItem(currentItem->row())->setText(category);
}

void RubricDialog::on_columnTitle_clicked()
{
    if(currentItem == nullptr) {
        return;
    }
    else if(currentItem->column() == cols) {
        return;
    }
    QString column = QInputDialog::getText(this, "Set Column",
                                           "Enter Column Title: ");
    ui->tableWidget->horizontalHeaderItem(currentItem->column())->setText(column);
}

void RubricDialog::on_cancelButton_clicked()
{
    close();
}

void RubricDialog::on_saveButton_clicked() //(!!! return values to main window !!!)
{
    //(!!! does not currently check to make sure values are correct !!!)
    //check point values (!!! currently does not account for extra credit !!!)
    int total = 0;
    bool ok;
    for(int i = 0; i < rows; i++) {
        total += ui->tableWidget->item(i, cols)->text().toInt(&ok);
    }
    if(total != ui->tableWidget->item(rows, cols)->text().toInt(&ok))
    {
        QMessageBox::warning(this, "Error",
                             "Please make sure category points add up to total before saving!");
        return;
    }
    else if(myRubric != nullptr) //modify existing rubric
    {
        for(int i = 0; i < rows; i++) {
            std::string category = ui->tableWidget->verticalHeaderItem(i)->text().toStdString();
            myRubric->get_rows()[i]->set_category(category);
            std::vector<std::string> descrips;

            for(int j = 0; j < cols; j++) {
                descrips.push_back(ui->tableWidget->item(i,j)->text().toStdString());
            }

            int points = ui->tableWidget->item(0, cols)->text().toInt(&ok);
            myRubric->get_rows()[i]->set_descriptions(descrips);
            myRubric->get_rows()[i]->set_max_points(points);
        }
        myRubric->set_max_points(ui->tableWidget->item(rows,cols)->text().toInt(&ok));

        close();
    }
    else //make new rubric (!!! currently column titles have no place to be saved !!!)
    {
        myRubric = new GARubric(title, ui->tableWidget->item(rows, cols)->text().toInt(&ok));
        for(int i = 0; i < rows; i++) {

            std::string category = ui->tableWidget->verticalHeaderItem(i)->text().toStdString();
            std::vector<std::string> descrips;

            for(int j = 0; j < cols; j++) {
                descrips.push_back(ui->tableWidget->item(i,j)->text().toStdString());
            }

            int points = ui->tableWidget->item(0, cols)->text().toInt(&ok);
            myRubric->add_row(category, descrips, points);
        }

        close();
    }
}
