#include "basescreen.h"
#include "ui_basescreen.h"

BaseScreen::BaseScreen(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::BaseScreen)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);
}

BaseScreen::~BaseScreen()
{
    delete ui;
}

void BaseScreen::on_actionBack_triggered()
{
    if(ui->stackedWidget->currentIndex() == 0)
    {
        return;
    }
    else if(ui->stackedWidget->currentIndex() == 1)
    {
        ui->stackedWidget->setCurrentIndex(0);
    }
    else if(ui->stackedWidget->currentIndex() == 2)
    {
        ui->stackedWidget->setCurrentIndex(1);
    }
    else if(ui->stackedWidget->currentIndex() == 3)
    {
        ui->stackedWidget->setCurrentIndex(2);
    }
    else if(ui->stackedWidget->currentIndex() == 4)
    {
        ui->stackedWidget->setCurrentIndex(0);
    }
    else
    {
        return;
    }
}

void BaseScreen::on_actionClasses_triggered()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void BaseScreen::on_actionRubrics_triggered()
{
    ui->stackedWidget->setCurrentIndex(4);
}

void BaseScreen::on_actionCurrent_Session_triggered()
{
    //current session window
}

void BaseScreen::on_actionQuit_triggered()
{
    //save and quit
    close();
}
