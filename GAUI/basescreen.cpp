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
//    for(int i = 0; i < ga.classes(); i++) {
//        ui->classListWidget->addItem(ga.classes()[].get_name());
//    }
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

void BaseScreen::on_selectFilePathButton_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this,
                                                    tr("Open File"),
                                                    "C://",
                                                    tr("Zip Files(*zip)"));
    if(!filePath.isEmpty()) {
        ui->fileEdit->setText(filePath);
    }
}

void BaseScreen::on_importButton_clicked()
{
    QString filePath = ui->fileEdit->text();
    ui->fileEdit->clear();
    //open files using given filePath and open grading session
}

//CLASSES PAGE SLOTS

void BaseScreen::on_deleteButton_clicked()
{
    //implement delete
}

void BaseScreen::on_selectButton_clicked()
{

}

void BaseScreen::on_addNew_clicked()
{

}
