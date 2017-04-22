#include "rubricdialog.h"
#include "ui_rubricdialog.h"


/**
 * @brief RubricDialog::RubricDialog is the constructor called when the user wants
 * to select an existing rubric.
 *
 * @param parent - basescreen
 * @param g - rubric to edit
 */
RubricDialog::RubricDialog(QWidget *parent, GARubric *g, GradingAssistant* ga) :
    QDialog(parent),
    ui(new Ui::RubricDialog)
{
    ui->setupUi(this);
    this->setWindowTitle(QString::fromStdString(g->get_title()));

    this->grading_assistant = ga;

    title = g->get_title();
    rows = g->get_rows().size();
    cols = g->get_rows().front()->get_descriptions().size();

    currentItem = nullptr;
    myRubric = g;

    //Set up extra credit area based on whether rubric currently allows for EC
    if(myRubric->get_ec() == nullptr)
    {
        ec = false;
        ui->label->setEnabled(false);
        ui->label_2->setEnabled(false);
        ui->descriptionEdit->setEnabled(false);
        ui->pointBox->setEnabled(false);
    }
    else {
        ui->extraCreditButton->setChecked(true);
        ui->descriptionEdit->setText(QString::fromStdString(myRubric->get_ec()->get_descriptions().front()));
        ui->pointBox->setValue(myRubric->get_ec()->get_max_points());
    }

    setup_table();
}


/**
 * @brief RubricDialog::RubricDialog is the constructor called when the user wants
 * to create a new rubric.
 *
 * @param parent - AddAssignmentDialog
 * @param t - new rubric title
 * @param r - number of rows
 * @param c - number of columns
 * @param p - max number of points
 */
RubricDialog::RubricDialog(QWidget *parent, QString t, int r, int c, GradingAssistant *ga) :
    QDialog(parent),
    ui(new Ui::RubricDialog)
{
    ui->setupUi(this);
    this->setWindowTitle(t);

    this->grading_assistant = ga;

    title = t.toStdString();
    rows = r;
    cols = c;

    currentItem = nullptr;
    myRubric = nullptr;

    ec = false;
    ui->label->setEnabled(false);
    ui->label_2->setEnabled(false);
    ui->descriptionEdit->setEnabled(false);
    ui->pointBox->setEnabled(false);

    setup_table();
}



//SETUP GUI



/**
 * @brief RubricDialog::setup_table sets up all the items in the table, including
 * headers, specifies which cells can be edited, and fills the rubric with the
 * ruric's current values if a rubric has been specified.
 */
void RubricDialog::setup_table()
{
    //disable header items
    ui->tableWidget->horizontalHeader()->setVisible(false);
    ui->tableWidget->verticalHeader()->setVisible(false);

    ui->tableWidget->setRowCount(rows + 2);
    ui->tableWidget->setColumnCount(cols + 2);

    //set up column headers
    for(int k = 0; k < (cols + 2); k++)
    {
        QTableWidgetItem *item = new QTableWidgetItem(2);

        if(k != cols+1) {
            QTableWidgetItem *item = new QTableWidgetItem(2);
            item->setFlags(item->flags() & ~Qt::ItemIsEditable);
            ui->tableWidget->setItem(0, k, item);
        }
        else {
            item->setText("Out Of: ");
            item->setFlags(item->flags() & ~Qt::ItemIsEditable);
            ui->tableWidget->setItem(0, cols+1, item);
        }
    }

    //fill grid with items
    for(int i = 1; i < rows+1; i++) {

        //set up row headers
        QTableWidgetItem *item = new QTableWidgetItem(2);

        if(myRubric == nullptr) {
            item->setText("Category");
        }
        else {
            item->setText(QString::fromStdString(myRubric->get_rows()[i-1]->get_category()));
        }

        ui->tableWidget->setItem(i, 0, item);

        //fill current row with items
        for(int j = 1; j < cols+2; j++) {

            //as long as we are not in the last column, add text description
            if(j != cols+1) {
                QTableWidgetItem *item = new QTableWidgetItem(2);
                if(myRubric != nullptr) {
                    item->setText(QString::fromStdString(myRubric->get_rows()[i-1]->get_descriptions()[j-1]));
                }
                ui->tableWidget->setItem(i, j, item);
            } //else, add int max score for category
            else {
                QTableWidgetItem *item = new QTableWidgetItem(1);
                if(myRubric != nullptr) {
                    item->setText(QString::number(myRubric->get_rows()[i-1]->get_max_points()));
                }
                ui->tableWidget->setItem(i, j, item);
            }
        }
    }

    //set up last row of table (blank except for total score)
    QTableWidgetItem *pointCategory = new QTableWidgetItem(2);
    pointCategory->setText("Total Points: ");
    pointCategory->setFlags(pointCategory->flags() & ~Qt::ItemIsEditable);
    ui->tableWidget->setItem(rows+1, 0, pointCategory);

    //make last row uneditable
    for(int m = 1; m < cols+1; m++)
    {
        QTableWidgetItem *item = new QTableWidgetItem(2);
        item->setFlags(item->flags() & ~Qt::ItemIsEditable);
        ui->tableWidget->setItem(rows+1, m, item);
    }

    //last item is total point value
    QTableWidgetItem *pointValue = new QTableWidgetItem(1);
    if(myRubric != nullptr) {
        pointValue->setText(QString::number(myRubric->get_max_points()));
    }
    ui->tableWidget->setItem(rows+1, cols+1, pointValue);

}


/**
 * @brief RubricDialog::~RubricDialog is the destructor for RubricDialog.
 */
RubricDialog::~RubricDialog()
{
    //delete all items in the tableWidget
    for(int i = 0; i < (rows+2); i++)
    {
        for(int j = 0; j < (cols+2); j++)
        {
            if(ui->tableWidget->item(i, j) != nullptr) {
                delete ui->tableWidget->item(i,j);
            }
        }
    }

    delete ui;
}



//SLOTS TO MODIFY RUBRIC



/**
 * @brief RubricDialog::on_addRowButton_clicked adds a row to the bottom of the
 * rubric.
 */
void RubricDialog::on_addRowButton_clicked()
{
    //add row
    ui->tableWidget->insertRow(rows+1);
    QTableWidgetItem *item = new QTableWidgetItem(2);
    item->setText("Category");
    ui->tableWidget->setItem(rows+1, 0, item);

    //fill new row with new cell items
    for(int i = 1; i < cols+1; i++) {
        QTableWidgetItem *item = new QTableWidgetItem(2);
        ui->tableWidget->setItem(rows+1, i, item);
    }

    //last item in row is a number
    QTableWidgetItem *num = new QTableWidgetItem(1);
    ui->tableWidget->setItem(rows+1, cols+1, num);

    rows++;
}


/**
 * @brief RubricDialog::on_deleteRowButton_clicked deletes the row the selected
 * item is in.
 */
void RubricDialog::on_deleteRowButton_clicked() {

    //make sure an item is selected, make sure the rubric has at least one row,
    //and don't delete the first or last row
    if(currentItem == nullptr || rows == 1) {
        return;
    }
    else if(currentItem->row() == (rows+1) || currentItem->row() == 0) {
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

    //if there is no column in the rubric, don't overwrite the score column
    int colToAdd;

    if(cols == 0) { //only score and category column is in rubric, put new column before score column
        colToAdd = 1;
    }
    else { //add normally
        colToAdd = cols+1;
    }

    //add columm
    ui->tableWidget->insertColumn(colToAdd);
    QTableWidgetItem *item = new QTableWidgetItem(2);
    item->setText("Column");
    ui->tableWidget->setHorizontalHeaderItem(colToAdd, item);

    //fill column with new cell items
    for(int i = 1; i < rows+1; i++) {
        QTableWidgetItem *item = new QTableWidgetItem(2);
        ui->tableWidget->setItem(i, colToAdd, item);
    }

    //last item and first item in column should not be editable
    QTableWidgetItem *blank = new QTableWidgetItem(2);
    blank->setFlags(blank->flags() & ~Qt::ItemIsEditable);
    QTableWidgetItem *blank2 = new QTableWidgetItem(2);
    blank2->setFlags(blank2->flags() & ~Qt::ItemIsEditable);
    ui->tableWidget->setItem(0, colToAdd, blank);
    ui->tableWidget->setItem(rows+1, colToAdd, blank2);

    cols++;
}


/**
 * @brief RubricDialog::on_deleteColumnButton_clicked deletes the row the selected
 * item is in.
 */
void RubricDialog::on_deleteColumnButton_clicked()
{
    //make sure an item is selected and don't delete first or last column
    if(currentItem == nullptr) {
        return;
    }
    else if(currentItem->column() == (cols+1) || currentItem->column() == 0) {
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
 * @brief RubricDialog::on_titleButton_clicked generates a QInputDialog to allow the user
 * to change the title of the rubric.
 */
void RubricDialog::on_titleButton_clicked()
{
    QString newTitle = QInputDialog::getText(this, "Change Title",
                                           "Enter New Title: ");
    title = newTitle.toStdString();
    this->setWindowTitle(newTitle);
}



//METHODS TO KEEP TRACK OF USER INPUT



/**
 * @brief RubricDialog::on_tableWidget_itemClicked keeps track of what the user
 * is selecting.
 *
 * @param item - item selected by user
 */
void RubricDialog::on_tableWidget_itemClicked(QTableWidgetItem *item)
{
    currentItem = item;
}


/**
 * @brief RubricDialog::on_extraCreditButton_stateChanged keeps track of whether
 * the user wants EC.
 *
 * @param arg1 - state of button, 0 for not checked, else for checked
 */
void RubricDialog::on_extraCreditButton_stateChanged(int arg1)
{
    if(arg1 == 0) //box is not checked and user does not want EC
    {
        ec = false;

        //disable EC fields
        ui->label->setEnabled(false);
        ui->label_2->setEnabled(false);
        ui->descriptionEdit->setEnabled(false);
        ui->pointBox->setEnabled(false);

        //clear fields as well
        ui->descriptionEdit->clear();
        ui->pointBox->setValue(0);
    }
    else //box is checked and user wants EC
    {
        ec = true;

        //enable EC fields
        ui->label->setEnabled(true);
        ui->label_2->setEnabled(true);
        ui->descriptionEdit->setEnabled(true);
        ui->pointBox->setEnabled(true);
    }
}


/**
 * @brief RubricDialog::on_tableWidget_itemChanged keeps track of whether the data
 * of a table item has been changed.
 *
 * Used to figure out if the user has updated any point values.
 *
 * @param item - item user has changed
 */
void RubricDialog::on_tableWidget_itemChanged(QTableWidgetItem *item)
{
    //if item is of interest (is in point column)
    if(item->tableWidget()->currentColumn() == cols+1)
    {
        //add up individual row values to get total
        int total = 0;
        bool ok;
        for(int i = 1; i < rows+1; i++) {
            total += ui->tableWidget->item(i, cols+1)->text().toInt(&ok);
        }

        //update total points
        ui->tableWidget->item(rows+1, cols+1)->setText(QString::number(total));
    }
}



//CLOSE DIALOG



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
    bool ok;
    if(myRubric != nullptr) /*modify existing rubric*/ {

        //reset title in case the user has changed it
        myRubric->set_title(title);
        myRubric->remove_all_rows();

        //save all row headers/descrips/points
        for(int i = 1; i < rows+1; i++) {

            //save category name
            std::string category = ui->tableWidget->item(i, 0)->text().toStdString();

            std::vector<std::string> descrips;

            //get all the descriptions
            for(int j = 1; j < cols+1; j++) {
                descrips.push_back(ui->tableWidget->item(i,j)->text().toStdString());
            }

            //get the max points for that row
            int points = ui->tableWidget->item(i, cols+1)->text().toInt(&ok);

            //reset values
            myRubric->add_row(category, descrips, points);
        }

        //if EC is checked, save this as well
        if(ui->extraCreditButton->isChecked()) {
            myRubric->set_ec("Extra Credit", ui->descriptionEdit->text().toStdString(),
                              ui->pointBox->value());
        }
        else {
            myRubric->remove_extra_credit();
        }
    } else /* make new rubric */{

        //make rubric
        myRubric = new GARubric(title);
        myRubric->set_grading_assistant(this->grading_assistant);

        //save all row headers/descrips/points
        for(int i = 1; i < rows+1; i++) {

            //save category name
            std::string category = ui->tableWidget->item(i, 0)->text().toStdString();

            std::vector<std::string> descrips;

            //get all the descriptions
            for(int j = 1; j < cols+1; j++) {
                descrips.push_back(ui->tableWidget->item(i,j)->text().toStdString());
            }

            //get the max points for that row
            int points = ui->tableWidget->item(i, cols+1)->text().toInt(&ok);

            //add new row with all these values
            myRubric->add_row(category, descrips, points);
        }

        //if EC is checked, save this as well
        if(ui->extraCreditButton->isChecked()) {
            myRubric->set_ec("Extra Credit", ui->descriptionEdit->text().toStdString(),
                              ui->pointBox->value());
        }
    }

    //close window
    close();
}



//GET METHOD



/**
 * @brief RubricDialog::get_rubric gets the created rubric (allows MainWindow to access
 * child dialog's information.
 *
 * @return myRubric - created rubric
 */
GARubric* RubricDialog::get_rubric() {
    return myRubric;
}

