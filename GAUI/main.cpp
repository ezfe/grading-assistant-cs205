#include "basescreen.h"
#include "flagdialog.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    BaseScreen w;
    w.show();

    return a.exec();
}

//#include <QtWidgets>

//#include "codetextedit.h"

//int main(int argv, char **args)
//{
//    QApplication app(argv, args);

//    CodeTextEdit *editor = new CodeTextEdit(0, "/home/sampsell/Desktop/StudentFiles/test.txt");
//    editor->setWindowTitle(QObject::tr("Code Editor Example"));
//    editor->show();

//    return app.exec();
//}
