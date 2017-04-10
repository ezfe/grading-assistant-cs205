#ifndef GRADINGSESSION_H
#define GRADINGSESSION_H

#include <QDialog>
#include <../grading-assistant/gradingassistant.h>
#include <../grading-assistant/gadata/gaclass.h>
#include <../grading-assistant/gadata/garubric.h>
#include <../grading-assistant/gadata/gaassignment.h>

namespace Ui {
class GradingSession;
}

class GradingSession : public QDialog
{
    Q_OBJECT

public:
    explicit GradingSession(QWidget *parent = 0);
    ~GradingSession();

private:
    Ui::GradingSession *ui;
};

#endif // GRADINGSESSION_H
