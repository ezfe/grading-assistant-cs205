#ifndef LABSTABWIDGET_H
#define LABSTABWIDGET_H

#include <QWidget>
#include <QFile>

namespace Ui {
class LabsTabWidget;
}

class LabsTabWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LabsTabWidget(QWidget *parent = 0);
    ~LabsTabWidget();

private:
    Ui::LabsTabWidget *ui;

    std::vector<std::pair<std::string, std::string>> fileNames;
};

#endif // LABSTABWIDGET_H
