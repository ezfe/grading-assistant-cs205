#include "labstabwidget.h"
#include "ui_labstabwidget.h"

LabsTabWidget::LabsTabWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LabsTabWidget)
{
    ui->setupUi(this);
}

LabsTabWidget::~LabsTabWidget()
{
    delete ui;
}
