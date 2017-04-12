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
        ui->annotationEdit->appendPlainText(QString::fromStdString(rubric->get_rows()[i]->
                                            get_category()));
        std::vector<GAAnnotation*> annotations = data->get_by_category(
                    rubric->get_rows()[i]->get_category());
        int subtractedPoints = 0;
        for(int j = 0; j < annotations.size(); j++) {
            ui->annotationEdit->appendPlainText(QString::fromStdString(
                                                    annotations[j]->get_title() +
                                                    ": " + annotations[j]->
                                                    get_description()) + "     " +
                                                    annotations[j]->get_points());
            if(annotations[j]->get_type() == "Error") {
                subtractedPoints -= annotations[j]->get_points();
            }
        }
        int earnedPoints = rubric->get_rows()[i]->get_max_points() - subtractedPoints;
//        ui->rubricWidget->cellWidget(i, cols)->setPlaceholderText(
//                    QString::number(earnedPoints) + " / "
//                    + QString::number(rubric->get_rows()[i]->get_max_points()));
        points.push_back(earnedPoints);
    }

    int totalEarned = 0;
    for(int k = 0; k < points.size(); k++) {
        totalEarned += points[k];
    }

//    ui->rubricWidget->cellWidget(rows,cols)->setPlaceholderText(QString::number(totalEarned +
//                                                                          " / " +
//                                                                          rubric->get_max_points());
}

void GradingDialog::setup_table() {
    ui->nameLabel->setText(QString::fromStdString(student->get_name()));

    rows = rubric->get_rows().size();
    cols = rubric->get_rows()[0]->get_descriptions().size();

    ui->rubricWidget->setRowCount(rows + 1);
    ui->rubricWidget->setColumnCount(cols + 1);


//    //set up column headers
//    for(int k = 0; k < (cols + 1); k++)
//    {
//        QTableWidgetItem *item = new QTableWidgetItem(2);
//        if(k != cols) {
//            QTableWidgetItem *item = new QTableWidgetItem(2);
//            item->setText("Column");
//            ui->tableWidget->setHorizontalHeaderItem(k, item);
//        }
//        else {
//            item->setText("Out Of: ");
//            item->setFlags(item->flags() & ~Qt::ItemIsEditable);
//            ui->tableWidget->setHorizontalHeaderItem(cols, item);
//        }
//    }

    //fill grid with items
    for(int i = 0; i < rows; i++) {

        //set up row headers
        QTableWidgetItem *item = new QTableWidgetItem(2);
        item->setText(QString::fromStdString(rubric->get_rows()[i]->get_category()));
        ui->rubricWidget->setVerticalHeaderItem(i, item);

        for(int j = 0; j < cols+1; j++) {
            if(j != cols) {
                QTableWidgetItem *item = new QTableWidgetItem(2);
                item->setText(QString::fromStdString(rubric->get_rows()[i]->get_descriptions()[j]));
                ui->rubricWidget->setItem(i, j, item);
            }
            else {
                QLineEdit *item = new QLineEdit();
                item->setPlaceholderText(" / " + QString::number(rubric->get_rows()[i]->get_max_points()));
                ui->rubricWidget->setCellWidget(i, j, item);
            }
        }
    }

    //set up last row of table
    QTableWidgetItem *pointCategory = new QTableWidgetItem(2);
    pointCategory->setText("Total Points: ");
    ui->rubricWidget->setVerticalHeaderItem(rows, pointCategory);

    //make last row uneditable
    for(int m = 0; m < cols; m++)
    {
        QTableWidgetItem *item = new QTableWidgetItem(2);
        item->setFlags(item->flags() & ~Qt::ItemIsEditable);
        ui->rubricWidget->setItem(rows, m, item);
    }

    //last item is total point value
    QLineEdit *pointValue = new QLineEdit();
    pointValue->setPlaceholderText(" / " + QString::number(rubric->get_max_points()));
    ui->rubricWidget->setCellWidget(rows, cols, pointValue);
}
