#include "addassignmentdialog.h"
#include "ui_addassignmentdialog.h"

AddAssignmentDialog::AddAssignmentDialog(QWidget *parent, GradingAssistant *g) :
    QDialog(parent),
    ui(new Ui::AddAssignmentDialog)
{
    ui->setupUi(this);
    ga = g;
    newAssignment = nullptr;

    ui->selectExistingButton->setChecked(true);

    for(int j = 0; j < ga->get_rubrics().size(); j++)
    {
        ui->rubricComboBox->addItem(QString::fromStdString(
                                        ga->get_rubrics()[j]->get_title()));
    }
}

AddAssignmentDialog::~AddAssignmentDialog()
{
    delete ui;
}

void AddAssignmentDialog::on_nextButton_clicked()
{
    newAssignment = new GAAssignment();
    newAssignment->set_title(ui->titleEdit->text().toStdString());
    newAssignment->set_description(ui->descriptionEdit->toPlainText().toStdString());

    if(ui->selectExistingButton->isChecked()) {
        GARubric *selectedRubric = ga->get_rubrics()[ui->rubricComboBox->currentIndex()];
        newAssignment->set_rubric(selectedRubric->copy());
    }
    else
    {
        rd = new RubricDialog(this, ui->rubricTitleEdit->text(), 1, 0);
        rd->exec();

        GARubric *newRubric = rd->get_rubric();

        if(newRubric == nullptr) { //user canceled
            return;
        }
        else {
            newAssignment->set_rubric(newRubric);
        }
        delete rd;
    }

    close();
}

GAAssignment* AddAssignmentDialog::get_new_assignment() {
    return newAssignment;
}
