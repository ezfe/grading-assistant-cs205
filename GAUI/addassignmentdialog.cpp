#include "addassignmentdialog.h"
#include "ui_addassignmentdialog.h"

AddAssignmentDialog::AddAssignmentDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddAssignmentDialog)
{
    ui->setupUi(this);
}

AddAssignmentDialog::~AddAssignmentDialog()
{
    delete ui;
}
