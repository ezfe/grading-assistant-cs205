#include "setupsessiondialog.h"
#include "ui_setupsessiondialog.h"

/*!
 * @brief Constructs a dialog to start a grading session
 * @param parent - BaseScreen
 * @param g - Grading Assistant
 */
SetupSessionDialog::SetupSessionDialog(QWidget *parent, GradingAssistant *g) :
    QDialog(parent),
    ui(new Ui::SetupSessionDialog)
{
    ui->setupUi(this);

    ga = g;
    selectedClass = nullptr;
    selectedAssignment = nullptr;

    //add class options to combo box
    for(GAClass* class_: ga->get_classes()) {
        ui->classComboBox->addItem(QString::fromStdString(class_->get_name() + " - " +
                                                          class_->get_semester() + " " +
                                                          class_->get_year()));
    }
}


/*!
 * @brief Destructs everything created for this dialog
 */
SetupSessionDialog::~SetupSessionDialog()
{
    delete ui;
}


/*!
 * @brief Get the class the user has selected
 * @return selectedClass
 */
GAClass* SetupSessionDialog::get_selected_class()
{
    return selectedClass;
}


/*!
 * @brief Get the assignment the user has selected
 * @return selectedAssignment
 */
GAAssignment* SetupSessionDialog::get_selected_assignment()
{
    return selectedAssignment;
}


/*!
 * @brief Slot called when user clicks the Open Session button; saves values to
 * be used when opening a grading session
 */
void SetupSessionDialog::on_openSessionButton_clicked()
{
    selectedClass = ga->get_classes()[ui->classComboBox->currentIndex()];

    //if the user has opted to add a new assignment, set up assignment dialog
    if(ui->assignmentComboBox->currentIndex() == selectedClass->get_assignments().size()) {
        aad = new AddAssignmentDialog(this, ga, selectedClass);
        aad->exec();

        //get the assignment the user has just created
        GAAssignment *assignment = aad->get_new_assignment();
        if(assignment == nullptr) {
            return;
        }
        else {
            //add this assignment to the selected class
            assignment->set_class(selectedClass);
            selectedClass->add_assignment(assignment);

            selectedAssignment = assignment;
        }
    }
    else { //user has picked a previously created assignment
        selectedAssignment = selectedClass->get_assignments()[ui->assignmentComboBox->currentIndex()];
    }

    close();
}


/*!
 * @brief Slot called when user changes the class selection; updates the
 * assignment list to reflect current class's assignments
 * @param index - current index
 */
void SetupSessionDialog::on_classComboBox_currentIndexChanged(int index)
{
    selectedClass = ga->get_classes()[index];

    //clear and refill assignment combo box
    ui->assignmentComboBox->clear();
    for(GAAssignment* assignment: selectedClass->get_assignments()) {
        ui->assignmentComboBox->addItem(QString::fromStdString(assignment->get_title()));
    }
    ui->assignmentComboBox->addItem("Add New");
}
