#include "gradingsession.h"
#include "ui_gradingsession.h"

GradingSession::GradingSession(QWidget *parent, GradingAssistant *ga, GAClass *c,
                               GARubric *r, GAAssignment *a) :
    QDialog(parent),
    ui(new Ui::GradingSession)
{
    gradingAssistant = ga;
    currentClass = c;
    currentRubric = r;
    currentAssignment = a;

    ui->setupUi(this);

    setup_dialog();
}

GradingSession::~GradingSession()
{
    delete ui;
}

void GradingSession::setup_dialog()
{
    ui->studentsToGrade->clear();

    for(GAStudent* s: currentClass->get_students()) {
        QListWidgetItem* item = new QListWidgetItem;
        item->setText(QString::fromStdString(s->get_name()));
        ui->studentsToGrade->addItem(item);
    }
}
void GradingSession::on_studentsToGrade_currentRowChanged(int currentRow)
{
    currentStudent = currentClass->get_students()[currentRow];
    currentAssignmentData = currentStudent->get_data(currentAssignment);
}

void GradingSession::on_flagTest_clicked()
{
    fd = new FlagDialog(this, gradingAssistant, currentRubric, "Error");
    fd->exec();

    if(fd->get_new_annotation() == nullptr) {
        return;
    }
    else {
        currentAssignmentData->add_annotation(fd->get_new_annotation());
    }

    delete fd;
}

void GradingSession::on_readyToGradeButton_clicked()
{
    gd = new GradingDialog(this, currentStudent, currentRubric, currentAssignmentData);
    gd->exec();

    delete gd;
}
