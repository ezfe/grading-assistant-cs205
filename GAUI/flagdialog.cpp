#include "flagdialog.h"
#include "ui_flagdialog.h"

FlagDialog::FlagDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FlagDialog)
{
    ui->setupUi(this);
}

FlagDialog::~FlagDialog()
{
    delete ui;
}
