#include "rubricdialog.h"
#include "ui_rubricdialog.h"

/**
 * @brief RubricDialog::RubricDialog is the constructo called when the user wants
 * to select an existing rubric.
 * @param parent - basescreen
 * @param g - rubric to edit
 */
RubricDialog::RubricDialog(QWidget *parent, GARubric *g) :
    QDialog(parent),
    ui(new Ui::RubricDialog)
{
    ui->setupUi(this);
    this->setWindowTitle(QString::fromStdString(g->get_title()));

    title = g->get_title();
    rows = g->get_rows().size();
    cols = g->get_rows().front()->get_descriptions().size();
    maxPoints = g->get_max_points();

    currentItem = nullptr;
    myRubric = g;

    setup_table();
}

/**
 * @brief RubricDialog::RubricDialog is the constructor called when the user wants
 * to create a new rubric.
 * @param parent - basescreen
 * @param t - new rubric title
 * @param r - number of rows
 * @param c - number of columns
 * @param p - max number of points
 */
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

/**
 * @brief RubricDialog::setup_table sets up all the items in the table, including
 * headers, specifies which cells can be edited, and fills the rubric with the
 * ruric's current values if a rubric has been specified.
 */
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
            item->setFlags(item->flags() & ~Qt::ItemIsEditable);
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
        item->setFlags(item->flags() & ~Qt::ItemIsEditable);
        ui->tableWidget->setItem(rows, m, item);
    }

    //last item is total point value
    QTableWidgetItem *pointValue = new QTableWidgetItem(1);
    pointValue->setText(QString::number(maxPoints));
    ui->tableWidget->setItem(rows, cols, pointValue);

}

/**
 * @brief RubricDialog::~RubricDialog is the destructor for RubricDialog.
 */
RubricDialog::~RubricDialog()
{
    //delete all items in the tableWidget
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

/**
 * @brief RubricDialog::on_addRowButton_clicked adds a row to the bottom of the
 * rubric.
 */
void RubricDialog::on_addRowButton_clicked()
{
    //add row
    ui->tableWidget->insertRow(rows);
    QTableWidgetItem *item = new QTableWidgetItem(2);
    item->setText("Category");
    ui->tableWidget->setVerticalHeaderItem(rows, item);

    //fill new row with new cell items
    for(int i = 0; i < cols; i++) {
        QTableWidgetItem *item = new QTableWidgetItem(2);
        ui->tableWidget->setItem(rows, i, item);
    }

    //last item in row is a number
    QTableWidgetItem *num = new QTableWidgetItem(1);
    ui->tableWidget->setItem(rows, cols, num);

    rows++;
}

/**
 * @brief RubricDialog::on_deleteRowButton_clicked deletes the row the selected
 * item is in.
 */
void RubricDialog::on_deleteRowButton_clicked() {
    //make sure an item is selected and don't delete the last row
    if(currentItem == nullptr) {
        return;
    }
    else if(currentItem->row() == (rows)) {
        return;
    }
    else //remove the row
    {
        ui->tableWidget->removeRow(currentItem->row());
        rows--;
        currentItem = nullptr;
    }
}

/**
 * @brief RubricDialog::on_addColumnButton_clicked adds a column to the right side
 * of the rubric.
 */
void RubricDialog::on_addColumnButton_clicked() {
    //add column
    ui->tableWidget->insertColumn(cols-1);
    QTableWidgetItem *item = new QTableWidgetItem(2);
    item->setText("Column");
    ui->tableWidget->setHorizontalHeaderItem(cols-1, item);

    //fill column with new cell items
    for(int i = 0; i < rows; i++) {
        QTableWidgetItem *item = new QTableWidgetItem(2);
        ui->tableWidget->setItem(i, cols-1, item);
    }

    //last item in column should not be editable
    QTableWidgetItem *blank = new QTableWidgetItem(2);
    blank->setFlags(item->flags() & ~Qt::ItemIsEditable);
    ui->tableWidget->setItem(rows, cols-1, blank);

    cols++;
}

/**
 * @brief RubricDialog::on_deleteColumnButton_clicked deletes the row the selected
 * item is in.
 */
void RubricDialog::on_deleteColumnButton_clicked()
{
    //make sure an item is selected and don't delete last column
    if(currentItem == nullptr) {
        return;
    }
    else if(currentItem->column() == (cols)) {
        return;
    }
    else //remove the column
    {
        ui->tableWidget->removeColumn(currentItem->column());
        cols--;
        currentItem = nullptr;
    }
}

/**
 * @brief RubricDialog::on_tableWidget_itemClicked keeps track of what the user
 * is selecting.
 * @param item - item selected by user
 */
void RubricDialog::on_tableWidget_itemClicked(QTableWidgetItem *item)
{
    currentItem = item;
}

/**
 * @brief RubricDialog::on_rowTitle_clicked generates a QInputDialog to allow the
 * user to change the title of a row.
 */
void RubricDialog::on_rowTitle_clicked()
{
    //make sure an item is selected and don't change the title of the last row
    if(currentItem == nullptr) {
        return;
    }
    else if(currentItem->row() == rows) {
        return;
    }

    //get input from user
    QString category = QInputDialog::getText(this, "Set Category",
                                             "Enter Category Title: ");
    ui->tableWidget->verticalHeaderItem(currentItem->row())->setText(category);
}

/**
 * @brief RubricDialog::on_columnTitle_clicked generates a QInputDialog to allow the
 * user to change the title of a column.
 */
void RubricDialog::on_columnTitle_clicked()
{
    //make sure an item is selected and don't change the title of the last column
    if(currentItem == nullptr) {
        return;
    }
    else if(currentItem->column() == cols) {
        return;
    }

    //get input from user
    QString column = QInputDialog::getText(this, "Set Column",
                                           "Enter Column Title: ");
    ui->tableWidget->horizontalHeaderItem(currentItem->column())->setText(column);
}

/**
 * @brief RubricDialog::on_cancelButton_clicked closes the dialog without saving
 * any data.
 */
void RubricDialog::on_cancelButton_clicked()
{
    close();
}

/**
 * @brief RubricDialog::on_saveButton_clicked saves all the data the user has inputted
 * and then closes the dialog.
 */
void RubricDialog::on_saveButton_clicked()
{
    //(!!! does not currently check to make sure values are correct !!!)
    //check point values (!!! currently does not account for extra credit !!!)
    int total = 0;
    bool ok;
    for(int i = 0; i < rows; i++) {
        total += ui->tableWidget->item(i, cols)->text().toInt(&ok);
    }
    if (total != ui->tableWidget->item(rows, cols)->text().toInt(&ok)) {
        QMessageBox::warning(this, "Error",
                             "Please make sure category points add up to total before saving!");
        return;
    } else if(myRubric != nullptr) /*modify existing rubric*/ {
        for(int i = 0; i < rows; i++) {
            std::string category = ui->tableWidget->verticalHeaderItem(i)->text().toStdString();
            myRubric->get_rows()[i]->set_category(category);
            std::vector<std::string> descrips;

            for(int j = 0; j < cols; j++) {
                descrips.push_back(ui->tableWidget->item(i,j)->text().toStdString());
            }

            int points = ui->tableWidget->item(i, cols)->text().toInt(&ok);
            myRubric->get_rows()[i]->set_descriptions(descrips);
            myRubric->get_rows()[i]->set_max_points(points);
        }

        close();
    } else /* make new rubric (!!! currently column titles have no place to be saved !!!)*/ {
        myRubric = new GARubric(title);
        for(int i = 0; i < rows; i++) {

            std::string category = ui->tableWidget->verticalHeaderItem(i)->text().toStdString();
            std::vector<std::string> descrips;

            for(int j = 0; j < cols; j++) {
                descrips.push_back(ui->tableWidget->item(i,j)->text().toStdString());
            }

            int points = ui->tableWidget->item(i, cols)->text().toInt(&ok);
            myRubric->add_row(category, descrips, points);
        }

        close();
    }
}

void RubricDialog::on_extraCreditButton_stateChanged(int arg1)
{
    //implement adding row of extra credit
}

GARubric* RubricDialog::get_rubric() {
    return myRubric;
}
