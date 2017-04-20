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
}

void GradingSession::on_studentsToGrade_currentRowChanged(int currentRow)
{
    currentStudent = currentClass->get_students()[currentRow];
    currentAssignmentData = currentStudent->get_data(currentAssignment);
    ui->currentStudentName->setText(QString::fromStdString(currentStudent->get_name()));

    //FileManager::import("/home/sampsell/Desktop/StudentFiles/littlen", gradingAssistant, currentAssignment);
    //std::string studentPath = FileManager::get_assignment_student_directory(currentAssignment, currentStudent);
    //FileManager::assure_directory_exists(studentPath);

    studentFiles = FileManager::get_files_in("/home/sampsell/Desktop/StudentFiles/littlen");

    ui->fileList->clear();

    for(std::pair<std::string, std::string> p: studentFiles) {
        QListWidgetItem* item = new QListWidgetItem;
        item->setText(QString::fromStdString(p.first));
        ui->fileList->addItem(item);
    }

    ui->fileList->setCurrentRow(0);
}

void GradingSession::on_fileList_currentRowChanged(int currentRow)
{
    if(currentRow < 0)
    {
        return;
    }
    std::pair<std::string, std::string> currentPair = studentFiles[currentRow];
    std::string currentPath = currentPair.second;
    ui->codeEdit->setup_text(currentPath);
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

void GradingSession::on_searchBox_textChanged(const QString &arg1)
{
    currentAnnotations = gradingAssistant->query_annotation(arg1.toStdString());

    //std::cerr << currentAnnotations.size() << std::endl;

    ui->annotationList->clear();

    for(GAAnnotation* a: currentAnnotations) {
        QListWidgetItem* item = new QListWidgetItem;
        item->setText(QString::fromStdString(a->get_title()));
        ui->annotationList->addItem(item);
    }
    selectedAnnotation = new GAAnnotation("GA_ANNOTATION_PROBLEM");
    selectedAnnotation->set_category("Self-esteem");
    selectedAnnotation->set_title("No Indenting");
    selectedAnnotation->set_description("Bad style.");
    selectedAnnotation->set_points(-3);
    ui->annotationList->addItem("No Indenting");
    currentAnnotations.push_back(selectedAnnotation);

    if(currentAnnotations.size() > 0) {
        ui->annotationList->setCurrentRow(0);
    }
}

void GradingSession::on_annotationList_currentRowChanged(int currentRow)
{
    if(currentRow >= 0) {
        selectedAnnotation = currentAnnotations[currentRow];
        print_preview();
    }
}

void GradingSession::print_preview()
{
    ui->previewEdit->clear();
    ui->previewEdit->setFontPointSize(14);
    ui->previewEdit->setFontWeight(QFont::Bold);

    bool change = true;

    for(int i = 0; i < currentRubric->get_rows().size(); i++)
    {
        if(selectedAnnotation->get_category() == currentRubric->get_rows()[i]->get_category()
                || selectedAnnotation->get_category() == "Extra Credit") {
            change = false;
        }
    }

    if(change == true) {
        selectedAnnotation->set_category(currentRubric->get_rows()[0]->get_category());
    }

    ui->previewEdit->append(QString::fromStdString(selectedAnnotation->get_category()));

    ui->previewEdit->setFontPointSize(11);
    ui->previewEdit->setFontWeight(QFont::Normal);

    ui->previewEdit->append(QString::fromStdString(selectedAnnotation->get_title()
                                                   + ": " +
                                                   selectedAnnotation->get_description()));

    if(selectedAnnotation->get_points() != 0) {
        ui->previewEdit->append(QString::number(selectedAnnotation->get_points()) + " points");
    }
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

void GradingSession::on_flagButton_clicked()
{
    if(currentStudent == nullptr || currentRubric->get_ec() == nullptr ||
            ui->codeEdit->get_current_line() == -1) {
        return;
    }

    if(selectedAnnotation == nullptr) {
        return;
    }
    else {
        GAAnnotation *newAnnotation = new GAAnnotation(selectedAnnotation->get_type());
        newAnnotation->set_title(selectedAnnotation->get_title());
        newAnnotation->set_category(selectedAnnotation->get_category());
        newAnnotation->set_description(selectedAnnotation->get_description());
        newAnnotation->set_points(selectedAnnotation->get_points());
        newAnnotation->set_filename(ui->fileList->currentItem()->text().toStdString());
        newAnnotation->set_line(ui->codeEdit->get_current_line());
        currentAssignmentData->add_annotation(newAnnotation);

        ui->searchBox->clear();
        ui->previewEdit->clear();
        ui->annotationList->clear();
        selectedAnnotation = nullptr;
    }
}

void GradingSession::on_editButton_clicked()
{
    if(currentStudent == nullptr || selectedAnnotation == nullptr ||
            ui->codeEdit->get_current_line() == -1) {
        return;
    }
    fd = new FlagDialog(this, gradingAssistant, currentRubric, selectedAnnotation);
    fd->exec();

    if(fd->get_new_annotation() == nullptr) {
        return;
    }
    else {
        selectedAnnotation = fd->get_new_annotation();

        GAAnnotation *newAnnotation = new GAAnnotation(selectedAnnotation->get_type());
        newAnnotation->set_title(selectedAnnotation->get_title());
        newAnnotation->set_category(selectedAnnotation->get_category());
        newAnnotation->set_description(selectedAnnotation->get_description());
        newAnnotation->set_points(selectedAnnotation->get_points());
        newAnnotation->set_filename(ui->fileList->currentItem()->text().toStdString());
        newAnnotation->set_line(ui->codeEdit->get_current_line());
        currentAssignmentData->add_annotation(newAnnotation);

        ui->searchBox->clear();
        ui->previewEdit->clear();
        ui->annotationList->clear();
    }

    delete fd;
}

void GradingSession::on_addNewButton_clicked()
{
    if(currentStudent == nullptr ||ui->codeEdit->get_current_line() == -1) {
        return;
    }
    fd = new FlagDialog(this, gradingAssistant, currentRubric, 1);
    fd->exec();

    if(fd->get_new_annotation() == nullptr) {
        return;
    }
    else {
        currentAssignmentData->add_annotation(fd->get_new_annotation());

        ui->searchBox->clear();
        ui->previewEdit->clear();
        ui->annotationList->clear();
        selectedAnnotation == nullptr;
    }

    delete fd;
}
