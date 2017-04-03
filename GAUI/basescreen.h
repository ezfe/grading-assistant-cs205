#ifndef BASESCREEN_H
#define BASESCREEN_H

#include <QMainWindow>
#include <QFileDialog>

namespace Ui {
class BaseScreen;
}

class BaseScreen : public QMainWindow
{
    Q_OBJECT

public:
    explicit BaseScreen(QWidget *parent = 0);
    ~BaseScreen();

private slots:
    void on_actionBack_triggered();

    void on_actionClasses_triggered();

    void on_actionRubrics_triggered();

    void on_actionCurrent_Session_triggered();

    void on_actionQuit_triggered();

    void on_selectFilePathButton_clicked();

    void on_importButton_clicked();

    void on_deleteButton_clicked();

    void on_selectButton_clicked();

    void on_addNew_clicked();

private:
    Ui::BaseScreen *ui;
    //GradingAssistant *ga
};

#endif // BASESCREEN_H
