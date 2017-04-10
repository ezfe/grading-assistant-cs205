#ifndef LABSTABWIDGET_H
#define LABSTABWIDGET_H

#include <QWidget>

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
};

#endif // LABSTABWIDGET_H
