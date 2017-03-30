#include "basescreen.h"
#include "ui_basescreen.h"

BaseScreen::BaseScreen(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::BaseScreen)
{
    ui->setupUi(this);
}

BaseScreen::~BaseScreen()
{
    delete ui;
}

void BaseScreen::on_actionBack_triggered()
{

}

void BaseScreen::on_actionClasses_triggered()
{

}

void BaseScreen::on_actionRubrics_triggered()
{

}

void BaseScreen::on_actionCurrent_Session_triggered()
{

}

void BaseScreen::on_actionQuit_triggered()
{

}
