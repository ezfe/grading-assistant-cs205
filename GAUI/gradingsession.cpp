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
    currentStudent = nullptr;

    ui->setupUi(this);

    setup_dialog();
}

GradingSession::~GradingSession()
{
    delete ui;
}

void GradingSession::setup_dialog()
{
    //clear list widget
    ui->studentsToGrade->clear();

    //put all current students in list
    for(GAStudent* s: currentClass->get_students()) {
        QListWidgetItem* item = new QListWidgetItem;
        item->setText(QString::fromStdString(s->get_name()));
        ui->studentsToGrade->addItem(item);
    }

    //start with first student
    ui->studentsToGrade->setCurrentRow(0);

    //for each student
    for(int i = 0; i < currentClass->get_students().size(); i++) {

        //make a new tab widget to put on the stack
        QTabWidget *tabWidget = new QTabWidget(ui->stackedWidget);

        //get directory path for current student

        std::vector<std::pair<std::string, std::string>> studentFiles =
                FileManager::get_files_in("/home/sampsell/Desktop/StudentFiles");

        //for each file in the student's folder
        for(int j = 0; j < studentFiles.size(); j++)
        {
            //set up the code widget
            std::string currentFile = studentFiles[j].second;
            CodeTextEdit *newCode = new CodeTextEdit(tabWidget, currentFile);

            //add the code widget to the tab
            tabWidget->addTab(newCode, QString::fromStdString(studentFiles[j].first));
        }

        //add the tab to the widget
        ui->stackedWidget->addWidget(tabWidget);
    }

    //start with first student
    ui->stackedWidget->setCurrentIndex(0);
}

void GradingSession::on_studentsToGrade_currentRowChanged(int currentRow)
{
    currentStudent = currentClass->get_students()[currentRow];
    currentAssignmentData = currentStudent->get_data(currentAssignment);
    ui->currentStudentName->setText(QString::fromStdString(currentStudent->get_name()));

    ui->stackedWidget->setCurrentIndex(currentRow);
}

void GradingSession::on_flagErrorButton_clicked()
{
    if(currentStudent == nullptr) {
        return;
    }
    fd = new FlagDialog(this, gradingAssistant, currentRubric, "GA_ANNOTATION_PROBLEM");
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
    if(currentStudent == nullptr) {
        return;
    }
    gd = new GradingDialog(this, currentStudent, currentRubric, currentAssignmentData);
    gd->exec();

    delete gd;
}

void GradingSession::on_flagCommentButton_clicked()
{
    if(currentStudent == nullptr) {
        return;
    }
    fd = new FlagDialog(this, gradingAssistant, currentRubric, "GA_ANNOTATION_COMMENT");
    fd->exec();

    if(fd->get_new_annotation() == nullptr) {
        return;
    }
    else {
        currentAssignmentData->add_annotation(fd->get_new_annotation());
    }

    delete fd;
}

void GradingSession::on_flagECButton_clicked()
{
    if(currentStudent == nullptr || currentRubric->get_ec() == nullptr) {
        return;
    }

    fd = new FlagDialog(this, gradingAssistant, currentRubric, "GA_ANNOTATION_EXTRACREDIT");
    fd->exec();

    if(fd->get_new_annotation() == nullptr) {
        return;
    }
    else {
        currentAssignmentData->add_annotation(fd->get_new_annotation());
    }

    delete fd;
}

void GradingSession::on_generateOutputButton_clicked()
{
    if(currentStudent == nullptr) {
        return;
    }

    QString filePath = QFileDialog::getExistingDirectory(this,
                                                    tr("Export Files to"),
                                                    "C://",
                                                    QFileDialog::ShowDirsOnly);
    if(filePath.isEmpty()) {
        return;
    }

    for(int i = 0; i < currentClass->get_students().size(); i++ ) {
        currentStudent = currentClass->get_students()[i];
        currentAssignmentData = currentStudent->get_data(currentAssignment);
        GAOutputFile *newFile = new GAOutputFile(filePath.toStdString(), currentAssignmentData);
        newFile->open_empty();
    }
    close();
}
