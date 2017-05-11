#include "gradingdialog.h"
#include "ui_gradingdialog.h"


//CONSTRUCTOR/DESTRUCTOR


/*!
 * @brief Sets up the dialog that allows users to see the results of a student's grading session
 * @param parent - GradingSession
 * @param s - current student
 * @param r - rubric being used for current assignment
 * @param d - student's data for current assignment
 */
GradingDialog::GradingDialog(QWidget *parent, GAStudent *s, GARubric *r,
                             GAAssignmentData *d) :
    QDialog(parent),
    ui(new Ui::GradingDialog)
{
    student = s;
    rubric = r;
    data = d;

    ui->setupUi(this);
    this->setWindowTitle("Grading Summary");

    //Setup widgets
    setup_dialog();
}


/*!
 * @brief Destructs everything created for this dialog
 */
GradingDialog::~GradingDialog()
{
    //delete all items in the table, depending on whether there is an EC row or not
    if(rubric->get_ec() == nullptr) {
        for(int i = 0; i < (rows+2); i++)
        {
            for(int j = 0; j < (cols+2); j++)
            {
                if(ui->rubricWidget->item(i, j) != nullptr) {
                    delete ui->rubricWidget->item(i,j);
                }
            }
        }
    }
    else {
        for(int i = 0; i < (rows+3); i++)
        {
            for(int j = 0; j < (cols+2); j++)
            {
                if(ui->rubricWidget->item(i, j) != nullptr) {
                    delete ui->rubricWidget->item(i,j);
                }
            }
        }
    }
    delete ui;
}


//SETUP VIEW


/**
 * @brief Setup everything associated with dialog
 */
void GradingDialog::setup_dialog() {

    ui->nameLabel->setText(QString::fromStdString(student->get_name()));

    //setup table and annotation widgets
    setup_annotations();
    setup_table();
}


/*!
 * @brief Fills in the text edit that shows the user all of the student's annotations
 */
void GradingDialog::setup_annotations() {

    //Go through annotations in each category
    for(GARubricRow* row: rubric->get_rows()) {

        //Print header for category
        ui->annotationEdit->setFontPointSize(14);
        ui->annotationEdit->setFontWeight(QFont::Bold);
        ui->annotationEdit->append(QString::fromStdString(row->get_category()));

        ui->annotationEdit->append("");

        //Get all the annotations in this category
        std::vector<GAAnnotation*> annotations = data->get_by_category(row->get_category());

        ui->annotationEdit->setFontPointSize(11);
        ui->annotationEdit->setFontWeight(QFont::Normal);

        //For each annotation found, print that annotation's information
        for(GAAnnotation* annotation: annotations) {
            ui->annotationEdit->append(QString::fromStdString(annotation->get_title() + ": " +
                                                              annotation->get_description()));
            if(annotation->get_points() != 0) {
                ui->annotationEdit->append(QString::number(annotation->get_points()));
            }
            ui->annotationEdit->append((QString::fromStdString(annotation->get_location())));
            ui->annotationEdit->append("");
        }
    }

    //If the rubric has EC, print that cateogry as well
    if(rubric->get_ec() != nullptr) {

        //Print header for that category
        ui->annotationEdit->setFontPointSize(14);
        ui->annotationEdit->setFontWeight(QFont::Bold);
        ui->annotationEdit->append("Extra Credit");
        ui->annotationEdit->append("");

        //Get EC annotations
        std::vector<GAAnnotation*> ec = data->get_by_category("Extra Credit");

        ui->annotationEdit->setFontPointSize(11);
        ui->annotationEdit->setFontWeight(QFont::Normal);

        //For each annotation found, print that annotation's information
        for(GAAnnotation* annotation: ec) {
            ui->annotationEdit->append(QString::fromStdString(annotation->get_title() + ": " +
                                                              annotation->get_description()));
            ui->annotationEdit->append("+" + QString::number(annotation->get_points()));
            ui->annotationEdit->append((QString::fromStdString(annotation->get_location())));
            ui->annotationEdit->append("");
        }
    }
}


/*!
 * @brief Fills in the table widget that show the user the current rubric + student scores
 */
void GradingDialog::setup_table() {

    //find number of rows and columns
    rows = rubric->get_rows().size();
    cols = rubric->get_rows()[0]->get_descriptions().size();

    //Get rid of the headers
    ui->rubricWidget->horizontalHeader()->setVisible(false);
    ui->rubricWidget->verticalHeader()->setVisible(false);
    ui->rubricWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    //If there is EC in the rubric, set up an extra row
    if(rubric->get_ec() != nullptr) {
        ui->rubricWidget->setRowCount(rows + 3);
    }
    else {
        ui->rubricWidget->setRowCount(rows + 2);
    }

    ui->rubricWidget->setColumnCount(cols + 2);

    //set up first row
    for(int i = 0; i < cols+2; i++) {
        QTableWidgetItem *item = new QTableWidgetItem(2);
        item->setFlags(item->flags() & ~Qt::ItemIsEditable);
        if(i == cols+1) {
            item->setText("Out of:");
        }
        ui->rubricWidget->setItem(0, i, item);
    }

    //fill grid with items
    for(int i = 1; i < rows+1; i++) {

        //set up row headers
        QTableWidgetItem *item = new QTableWidgetItem(2);
        item->setText(QString::fromStdString(rubric->get_rows()[i-1]->get_category()));
        ui->rubricWidget->setItem(i, 0, item);

        //fill in descriptions + points
        for(int j = 1; j < cols+2; j++) {
            if(j != cols+1) {
                QTableWidgetItem *item = new QTableWidgetItem(2);
                item->setText(QString::fromStdString(rubric->get_rows()[i-1]->
                              get_descriptions()[j-1]));
                item->setFlags(item->flags() & ~Qt::ItemIsEditable);
                ui->rubricWidget->setItem(i, j, item);
            }
            else { //in point column, print earned and total possible points
                QTableWidgetItem *item = new QTableWidgetItem(2);
                item->setText(QString::number(data->calculate_score(rubric->get_rows()[i-1]))
                        + " / "
                        + QString::number(rubric->get_rows()[i-1]->get_max_points()));
                item->setFlags(item->flags() & ~Qt::ItemIsEditable);
                ui->rubricWidget->setItem(i, j, item);
            }
        }
    }

    //if the ruric does not have EC, just set up the last row
    if(rubric->get_ec() == nullptr) {

        //set up last row of table
        QTableWidgetItem *pointCategory = new QTableWidgetItem(2);
        pointCategory->setText("Total Points: ");
        pointCategory->setFlags(pointCategory->flags() & ~Qt::ItemIsEditable);
        ui->rubricWidget->setItem(rows+1, 0, pointCategory);

        //make last row uneditable
        for(int m = 1; m < cols+1; m++)
        {
            QTableWidgetItem *item = new QTableWidgetItem(2);
            item->setFlags(item->flags() & ~Qt::ItemIsEditable);
            ui->rubricWidget->setItem(rows+1, m, item);
        }

        //last item is points earned and total possible point value
        QTableWidgetItem *pointValue = new QTableWidgetItem(2);
        pointValue->setText(QString::number(data->calculate_score())
                            + " / " + QString::number(rubric->get_max_points()));
        pointValue->setFlags(pointValue->flags() & ~Qt::ItemIsEditable);
        ui->rubricWidget->setItem(rows+1, cols+1, pointValue);
    }
    else { //add in the EC row along with the last row

        //set up ec row of table
        QTableWidgetItem *echeader = new QTableWidgetItem(2);
        echeader->setText("Extra Credit");
        ui->rubricWidget->setItem(rows+1, 0, echeader);

        //make EC row uneditable except for first
        for(int n = 1; n < cols+1; n++)
        {
            QTableWidgetItem *item = new QTableWidgetItem(2);
            if(n == 1) {
                item->setText(QString::fromStdString(rubric->get_ec()->get_descriptions().front()));
            }
            item->setFlags(item->flags() & ~Qt::ItemIsEditable);
            ui->rubricWidget->setItem(rows+1, n, item);
        }

        //last item is ec value, print earned and total possible points
        QTableWidgetItem *ecPointValue = new QTableWidgetItem(2);
        ecPointValue->setText(QString::number(data->calculate_score(rubric->get_ec()))
                              + " / " + QString::number(rubric->get_ec()->get_max_points()));
        ecPointValue->setFlags(ecPointValue->flags() & ~Qt::ItemIsEditable);
        ui->rubricWidget->setItem(rows+1, cols+1, ecPointValue);

        //set up last row of table
        QTableWidgetItem *pointCategory = new QTableWidgetItem(2);
        pointCategory->setText("Total Points: ");
        pointCategory->setFlags(pointCategory->flags() & ~Qt::ItemIsEditable);
        ui->rubricWidget->setItem(rows+2, 0, pointCategory);

        //make last row uneditable
        for(int m = 1; m < cols+1; m++)
        {
            QTableWidgetItem *item = new QTableWidgetItem(2);
            item->setFlags(item->flags() & ~Qt::ItemIsEditable);
            ui->rubricWidget->setItem(rows+2, m, item);
        }

        //last item is points earned and total possible point value
        QTableWidgetItem *pointValue = new QTableWidgetItem(2);
        pointValue->setText(QString::number(data->calculate_score())
                            + " / " + QString::number(rubric->get_max_points()));
        pointValue->setFlags(pointValue->flags() & ~Qt::ItemIsEditable);
        ui->rubricWidget->setItem(rows+2, cols+1, pointValue);
    }

}


//KEEP TRACK OF USER INPUT


/**
 * @brief Slot called when user has double clicked an item, used to allow user to override total
 * score if that item is double clicked
 * @param row - row of double clicked item
 * @param column - column of double clicked item
 */
void GradingDialog::on_rubricWidget_cellDoubleClicked(int row, int column)
{
    //if the user is in the total points box (location changes depending on EC row)
    if((rubric->get_ec() != nullptr && (column == cols+1 && row == (rows+2)))
            || (rubric->get_ec() == nullptr && (column == cols+1 && row == rows+1)))
    {
        //Set up input dialog
        QInputDialog *inputDialog = new QInputDialog(this);

        inputDialog->setParent(this);
        inputDialog->setWindowTitle("Override Score");
        inputDialog->setTextValue("Enter New Score: ");
        inputDialog->setInputMode(QInputDialog::IntInput);
        inputDialog->setCancelButtonText("Reset Score");

        //execute dialog
        int choice = inputDialog->exec();

        //if user wants to override score
        if(choice == 1) {
            //override score
            data->override_score(inputDialog->intValue());

            //recalculate/reprint total points box, depending on if there is an EC row
            if(rubric->get_ec() != nullptr) {
                ui->rubricWidget->item(rows+2, cols+1)->setText(QString::number
                                                                (data->calculate_score())
                                                                + " / " + QString::number
                                                                (rubric->get_max_points()));
            }
            else
            {
                ui->rubricWidget->item(rows+1, cols+1)->setText(QString::number
                                                                (data->calculate_score())
                                                                + " / " + QString::number
                                                                (rubric->get_max_points()));
            }
        }
        else { //user want to reset system to use calculated scoring
            //reset
            data->override_score(-1);
        }
        //recalculate/reprint total points box, depending on if there is an EC row
        if(rubric->get_ec() != nullptr) {
            ui->rubricWidget->item(rows+2, cols+1)->setText(QString::number
                                                            (data->calculate_score())
                                                            + " / " + QString::number
                                                            (rubric->get_max_points()));
        }
        else {
            ui->rubricWidget->item(rows+1, cols+1)->setText(QString::number
                                                            (data->calculate_score())
                                                            + " / " + QString::number
                                                            (rubric->get_max_points()));
        }
    }
}


/**
 * @brief Slot called when user clicks the Save button, closes dialog to return user to GradingSession
 */
void GradingDialog::on_saveButton_clicked()
{
    close();
}
