#include "gradingsession.h"
#include "ui_gradingsession.h"

GradingSession::GradingSession(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GradingSession)
{
    ui->setupUi(this);
}

GradingSession::~GradingSession()
{
    delete ui;
}
