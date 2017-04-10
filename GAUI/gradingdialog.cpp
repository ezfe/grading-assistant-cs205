#include "gradingdialog.h"
#include "ui_gradingdialog.h"

GradingDialog::GradingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GradingDialog)
{
    ui->setupUi(this);
}

GradingDialog::~GradingDialog()
{
    delete ui;
}
