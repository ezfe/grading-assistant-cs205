#include "setupsessiondialog.h"
#include "ui_setupsessiondialog.h"

SetupSessionDialog::SetupSessionDialog(QWidget *parent, GradingAssistant *g) :
    QDialog(parent),
    ui(new Ui::SetupSessionDialog)
{
    ui->setupUi(this);

    ga = g;
    selectedClass = nullptr;
    selectedRubric = nullptr;
    selectedAssignment = nullptr;

    for(int i = 0; i < ga->get_classes().size(); i++)
    {
        ui->classComboBox->addItem(QString::fromStdString(
                                       ga->get_classes()[i]->get_name()));
    }
    for(int j = 0; j < ga->get_rubrics().size(); j++)
    {
        ui->rubricComboBox->addItem(QString::fromStdString(
                                        ga->get_rubrics()[j]->get_title()));
    }
}

SetupSessionDialog::~SetupSessionDialog()
{
    delete ui;
}

void SetupSessionDialog::on_openSessionButton_clicked()
{
    selectedClass = ga->get_classes()[ui->classComboBox->currentIndex()];
    selectedRubric = ga->get_rubrics()[ui->rubricComboBox->currentIndex()];
    selectedAssignment = selectedClass->get_assignments()
            [ui->assignmentComboBox->currentIndex()];
    close();
}

GAClass* SetupSessionDialog::get_selected_class()
{
    return selectedClass;
}

GARubric* SetupSessionDialog::get_selected_rubric()
{
    return selectedRubric;
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
}
