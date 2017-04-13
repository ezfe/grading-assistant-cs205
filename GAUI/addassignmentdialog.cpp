#include "addassignmentdialog.h"
#include "ui_addassignmentdialog.h"

AddAssignmentDialog::AddAssignmentDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddAssignmentDialog)
{
    ui->setupUi(this);
    newAssignment = nullptr;
}

AddAssignmentDialog::~AddAssignmentDialog()
{
    delete ui;
}

void AddAssignmentDialog::on_addClassButton_clicked()
{
    newAssignment = new GAAssignment();
    newAssignment->set_title(ui->titleEdit->text().toStdString());
    newAssignment->set_description(ui->descriptionEdit->toPlainText().toStdString());
    close();
}

GAAssignment* AddAssignmentDialog::get_new_assignment() {
    return newAssignment;
}
