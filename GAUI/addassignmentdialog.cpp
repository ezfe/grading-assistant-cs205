#include "addassignmentdialog.h"
#include "ui_addassignmentdialog.h"

/*!
 * @brief Constructs a dialog to add a new assignment
 * @param parent - BaseScreen
 * @param g - Grading Assistant
 */
AddAssignmentDialog::AddAssignmentDialog(QWidget *parent, GradingAssistant *g) :
    QDialog(parent),
    ui(new Ui::AddAssignmentDialog)
{
    ui->setupUi(this);
    ga = g;
    newAssignment = nullptr;

    //Set default selections
    if(ga->get_rubrics().size() == 0) {
        ui->selectExistingButton->setEnabled(false);
        ui->addNewButton->setChecked(true);
        ui->rubricComboBox->setEnabled(false);
    }
    else {
        ui->selectExistingButton->setChecked(true);
        ui->rubricTitleEdit->setEnabled(false);
    }

    //Add possible rubric selections to combo box
    for(int j = 0; j < ga->get_rubrics().size(); j++)
    {
        ui->rubricComboBox->addItem(QString::fromStdString(
                                        ga->get_rubrics()[j]->get_title()));
    }
}


/*!
 * @brief Destructs everything created for this dialog
 */
AddAssignmentDialog::~AddAssignmentDialog()
{
    delete ui;
}


/*!
 * @brief Gets the newly created assignment
 * @return  newAssignment
 */
GAAssignment* AddAssignmentDialog::get_new_assignment() {
    return newAssignment;
}


/*!
 * @brief Slot called when user clicks Next button
 */
void AddAssignmentDialog::on_nextButton_clicked()
{
    //Create a new assignment with title + description
    newAssignment = new GAAssignment(this->ga);
    newAssignment->set_grading_assistant(ga);
    newAssignment->set_title(ui->titleEdit->text().toStdString());
    newAssignment->set_description(ui->descriptionEdit->toPlainText().toStdString());

    //if the user wants to select an existing rubric, set assignment's rubric to copy
    if(ui->selectExistingButton->isChecked()) {
        GARubric *selectedRubric = ga->get_rubrics()[ui->rubricComboBox->currentIndex()];
        newAssignment->set_rubric(selectedRubric->copy());
    }
    else //create a RubricDialog to create a new rubric
    {
        rd = new RubricDialog(this, ui->rubricTitleEdit->text(), 1, 0, ga);


        rd->exec();

        GARubric *newRubric = rd->get_rubric();

        if(newRubric == nullptr) { //user canceled, do nothing
            return;
        }
        else {
            newAssignment->set_rubric(newRubric); //set rubric to created rubric
        }
        delete rd; //delete dialog
    }

    close();
}


/*!
 * @brief Slot called when user clicks the Select Existing button
 */
void AddAssignmentDialog::on_selectExistingButton_clicked()
{
    //Disable add new, enable select existing
    ui->rubricTitleEdit->setDisabled(true);
    ui->rubricComboBox->setEnabled(true);
}


/*!
 * @brief Slot called when user clicks the Add New button
 */
void AddAssignmentDialog::on_addNewButton_clicked()
{
    //Disable selecting existing, enable add new
    ui->rubricTitleEdit->setEnabled(true);
    ui->rubricComboBox->setDisabled(true);
}
