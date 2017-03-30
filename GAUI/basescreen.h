#ifndef BASESCREEN_H
#define BASESCREEN_H

#include <QMainWindow>

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

private:
    Ui::BaseScreen *ui;
};

#endif // BASESCREEN_H
