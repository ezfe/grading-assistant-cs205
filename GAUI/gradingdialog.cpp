#include "gradingdialog.h"
#include "ui_gradingdialog.h"

GradingDialog::GradingDialog(QWidget *parent, GAStudent *s, GARubric *r,
                             GAAssignmentData *d) :
    QDialog(parent),
    ui(new Ui::GradingDialog)
{
    student = s;
    rubric = r;
    data = d;

    ui->setupUi(this);

    setup_dialog();
    setup_annotations();
}

GradingDialog::~GradingDialog()
{
    delete ui;
}

void GradingDialog::setup_dialog() {

   setup_table();

}

void GradingDialog::setup_annotations() {

    for(int i = 0; i < rubric->get_rows().size(); i++) {

        ui->annotationEdit->setFontPointSize(14);
        ui->annotationEdit->setFontWeight(QFont::Bold);
        ui->annotationEdit->append(QString::fromStdString(rubric->get_rows()[i]->
                                            get_category()));

        ui->annotationEdit->append("");
        std::vector<GAAnnotation*> annotations = data->get_by_category(
                    rubric->get_rows()[i]->get_category());

        ui->annotationEdit->setFontPointSize(11);
        ui->annotationEdit->setFontWeight(QFont::Normal);
        for(int j = 0; j < annotations.size(); j++) {
            ui->annotationEdit->append(QString::fromStdString(annotations[j]->get_title()
                                                    + ": " + annotations[j]->
                                                    get_description()));
            if(annotations[j]->get_points() != 0) {
                ui->annotationEdit->append(QString::number(annotations[j]->get_points()));
            }
            ui->annotationEdit->append((QString::fromStdString(annotations[j]->get_location())));
            ui->annotationEdit->append("");
        }
    }

    if(rubric->get_ec() != nullptr) {
        ui->annotationEdit->setFontPointSize(14);
        ui->annotationEdit->setFontWeight(QFont::Bold);
        ui->annotationEdit->append("Extra Credit");
        ui->annotationEdit->append("");

        std::vector<GAAnnotation*> ec = data->get_by_category("Extra Credit");

        ui->annotationEdit->setFontPointSize(11);
        ui->annotationEdit->setFontWeight(QFont::Normal);

        for(int k = 0; k < ec.size(); k++) {
            ui->annotationEdit->append(QString::fromStdString(ec[k]->get_title()
                                                              + ": " + ec[k]->
                                                              get_description()));
            ui->annotationEdit->append("+" + QString::number(ec[k]->get_points()));
            ui->annotationEdit->append((QString::fromStdString(ec[k]->get_location())));
            ui->annotationEdit->append("");
        }
    }
}

void GradingDialog::setup_table() {
    ui->nameLabel->setText(QString::fromStdString(student->get_name()));

    rows = rubric->get_rows().size();
    cols = rubric->get_rows()[0]->get_descriptions().size();
    ui->rubricWidget->horizontalHeader()->setVisible(false);
    ui->rubricWidget->verticalHeader()->setVisible(false);

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

        for(int j = 1; j < cols+2; j++) {
            if(j != cols+1) {
                QTableWidgetItem *item = new QTableWidgetItem(2);
                item->setText(QString::fromStdString(rubric->get_rows()[i-1]->get_descriptions()[j-1]));
                item->setFlags(item->flags() & ~Qt::ItemIsEditable);
                ui->rubricWidget->setItem(i, j, item);
            }
            else {
                QTableWidgetItem *item = new QTableWidgetItem(2);
                item->setText(QString::number(data->calculate_score(rubric->get_rows()[i-1]))
                              + " / "
                              + QString::number(rubric->get_rows()[i-1]->get_max_points()));
                item->setFlags(item->flags() & ~Qt::ItemIsEditable);
                ui->rubricWidget->setItem(i, j, item);
            }
        }
    }

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

        //last item is total point value
        QTableWidgetItem *pointValue = new QTableWidgetItem(2);
        pointValue->setText(QString::number(data->calculate_score())
                    + " / " + QString::number(rubric->get_max_points()));
        pointValue->setFlags(pointValue->flags() & ~Qt::ItemIsEditable);
        ui->rubricWidget->setItem(rows+1, cols+1, pointValue);
    }
    else {
        //set up ec row of table
        QTableWidgetItem *echeader = new QTableWidgetItem(2);
        echeader->setText("Extra Credit");
        ui->rubricWidget->setItem(rows+1, 0, echeader);

        //make last row uneditable except for first
        for(int n = 1; n < cols+1; n++)
        {
            QTableWidgetItem *item = new QTableWidgetItem(2);
            if(n == 1) {
                item->setText(QString::fromStdString(rubric->get_ec()->get_descriptions().front()));
            }
            item->setFlags(item->flags() & ~Qt::ItemIsEditable);
            ui->rubricWidget->setItem(rows+1, n, item);
        }

        //last item is ec value
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

        //last item is total point value
        QTableWidgetItem *pointValue = new QTableWidgetItem(2);
        pointValue->setText(QString::number(data->calculate_score())
                    + " / " + QString::number(rubric->get_max_points()));
        pointValue->setFlags(pointValue->flags() & ~Qt::ItemIsEditable);
        ui->rubricWidget->setItem(rows+2, cols+1, pointValue);
    }

}

void GradingDialog::on_rubricWidget_cellDoubleClicked(int row, int column)
{
    if((rubric->get_ec() != nullptr && (column == cols+1 && row == (rows+2)))
            || rubric->get_ec() == nullptr && (column == cols+1 && row == rows+1))
    {
        QInputDialog *inputDialog = new QInputDialog(this);
        inputDialog->setParent(this);
        inputDialog->setWindowTitle("Override Score");
        inputDialog->setTextValue("Enter New Score: ");
        inputDialog->setInputMode(QInputDialog::IntInput);
        inputDialog->setCancelButtonText("Reset Score");

        int choice = inputDialog->exec();

        if(choice == 1) {
            data->override_score(inputDialog->intValue());
            if(rubric->get_ec() != nullptr) {
                ui->rubricWidget->item(rows+2, cols+1)->setText(QString::number(data->calculate_score())
                                                          + " / " + QString::number(rubric->get_max_points()));
            }
            else
            {
                ui->rubricWidget->item(rows+1, cols+1)->setText(QString::number(data->calculate_score())
                                                          + " / " + QString::number(rubric->get_max_points()));
            }
        }
        else {
            data->override_score(-1);
        }
        if(rubric->get_ec() != nullptr) {
            ui->rubricWidget->item(rows+2, cols+1)->setText(QString::number(data->calculate_score())
                                                          + " / " + QString::number(rubric->get_max_points()));
        }
        else {
            ui->rubricWidget->item(rows+1, cols+1)->setText(QString::number(data->calculate_score())
                                                          + " / " + QString::number(rubric->get_max_points()));
        }
    }
}

void GradingDialog::on_saveButton_clicked()
{
    close();
}
