#include "rubricdialog.h"
#include "ui_rubricdialog.h"

RubricDialog::RubricDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RubricDialog)
{
    ui->setupUi(this);
}

RubricDialog::~RubricDialog()
{
    delete ui;
}
