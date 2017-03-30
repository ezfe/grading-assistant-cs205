#include "basescreen.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    BaseScreen w;
    w.show();

    return a.exec();
}
