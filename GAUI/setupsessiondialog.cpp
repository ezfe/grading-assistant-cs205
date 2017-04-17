#include "setupsessiondialog.h"
#include "ui_setupsessiondialog.h"

SetupSessionDialog::SetupSessionDialog(QWidget *parent, GradingAssistant *g) :
    QDialog(parent),
    ui(new Ui::SetupSessionDialog)
{
    ui->setupUi(this);

    ga = g;
    selectedClass = nullptr;
    selectedAssignment = nullptr;

    for(int i = 0; i < ga->get_classes().size(); i++)
    {
        ui->classComboBox->addItem(QString::fromStdString(
                                       ga->get_classes()[i]->get_name()));
    }
}

SetupSessionDialog::~SetupSessionDialog()
{
    delete ui;
}

void SetupSessionDialog::on_openSessionButton_clicked()
{
    selectedClass = ga->get_classes()[ui->classComboBox->currentIndex()];
    if(ui->assignmentComboBox->currentIndex() == selectedClass->get_assignments().size()) {
        aad = new AddAssignmentDialog(this, ga);
        aad->exec();

        GAAssignment *assignment = aad->get_new_assignment();
        if(assignment == nullptr) {
            return;
        }
        else {
            assignment->set_class(selectedClass);
            selectedClass->add_assignment(assignment);

            selectedAssignment = assignment;
        }
    }
    else {
        selectedAssignment = selectedClass->get_assignments()[ui->assignmentComboBox->currentIndex()];
    }

    close();
}

GAClass* SetupSessionDialog::get_selected_class()
{
    return selectedClass;
}

GAAssignment* SetupSessionDialog::get_selected_assignment()
{
    return selectedAssignment;
}

void SetupSessionDialog::on_classComboBox_currentIndexChanged(int index)
{
    selectedClass = ga->get_classes()[index];
    ui->assignmentComboBox->clear();
    for(int k = 0; k < selectedClass->get_assignments().size(); k++)
    {
        ui->assignmentComboBox->addItem(QString::fromStdString(
                                            selectedClass->get_assignments()[k]->get_title()));
    }
    ui->assignmentComboBox->addItem("Add New");
}
