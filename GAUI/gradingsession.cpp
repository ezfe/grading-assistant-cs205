#include "gradingsession.h"
#include "ui_gradingsession.h"

/*!
 * @brief Sets up the UI for a grading session
 * @param parent - BaseScreen
 * @param ga - GradingAssistant
 * @param c - Class being graded
 * @param r - Rubric being used
 * @param a - Assignment being graded
 */
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
    selectedAnnotation = nullptr;

    ui->setupUi(this);

    //connect signal: when user clicks on code edit, let its parent widget know what
    //the new line number is
    connect(ui->codeEdit, SIGNAL(selectionChanged()), this, SLOT(update_selection()));

    ui->codeEdit->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->codeEdit, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(show_context_menu(QPoint)));

    setup_dialog();
}


/**
 * @brief Destructs everything created for this dialog
 */
GradingSession::~GradingSession()
{
    delete ui;
}

/*!
 * @brief Fills the dialog's child widgets with appropriate data
 */
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

    //start with first student selected
    ui->studentsToGrade->setCurrentRow(0);
}


/*!
 * @brief Slot representing the user changing the selected student from the list
 * of students
 * @param currentRow - row clicked
 */
void GradingSession::on_studentsToGrade_currentRowChanged(int currentRow)
{
    //switch current student/data
    currentStudent = currentClass->get_students()[currentRow];
    currentAssignmentData = currentStudent->get_data(currentAssignment);
    ui->currentStudentName->setText(QString::fromStdString(currentStudent->get_name()));

    //GET STUDENTS FILES

//    std::string studentPath = FileManager::get_assignment_student_directory(currentAssignment, currentStudent);
//    FileManager::assure_directory_exists(studentPath);

//    studentFiles = FileManager::get_files_in(studentPath);
    studentFiles = FileManager::get_files_in("/home/sampsell/Desktop/StudentFiles/littlen");

    //clear, then refill list of files in list widget
    ui->fileList->clear();

    for(std::pair<std::string, std::string> p: studentFiles) {
        QListWidgetItem* item = new QListWidgetItem;
        item->setText(QString::fromStdString(p.first));
        ui->fileList->addItem(item);
    }

    //start with the first file selected
    ui->fileList->setCurrentRow(0);
}


/*!
 * @brief Slot representing the user changing the selected file from the list of
 * files
 * @param currentRow - row clicked
 */
void GradingSession::on_fileList_currentRowChanged(int currentRow)
{
    //if none selected (happens briefly when new student is selected), do nothing
    if(currentRow < 0){
        return;
    }

    //clear any annotation from the preview
    ui->previewEdit->clear();

    //get that file
    std::pair<std::string, std::string> currentPair = studentFiles[currentRow];
    std::string currentPath = currentPair.second;
    currentFile = currentPair.first;

    //set up the code edit
    ui->codeEdit->setup_text(currentPath);

    //get all the annotations for that file
    std::vector<int> lineNumbers = currentAssignmentData->get_line_numbers(currentFile);

    //if there are annotations, highlight them on the edit
    if(lineNumbers.size() != 0) {
        ui->codeEdit->setup_highlights(lineNumbers);
    }
}


/*!
 * @brief Slot representing the user changing the selected annotation in the
 * annotation list
 * @param currentRow - row selected
 */
void GradingSession::on_annotationList_currentRowChanged(int currentRow)
{
    //as long as something is selected
    if(currentRow >= 0) {
        selectedAnnotation = currentAnnotations[currentRow];
        print_preview();
    }
}


/*!
 * @brief Slot representing the user pressing the Ready to Grade button
 */
void GradingSession::on_readyToGradeButton_clicked()
{
    //if no student has been selected, do nothing
    if(currentStudent == nullptr) {
        return;
    }

    //open a grading dialog with this student's information
    gd = new GradingDialog(this, currentStudent, currentRubric, currentAssignmentData);

    gd->setAttribute(Qt::WA_DeleteOnClose);
    gd->show();
}


/*!
 * @brief Slot called when user clicks on the Generate Output button
 */
void GradingSession::on_generateOutputButton_clicked()
{
    //if there is no student selected, do nothing
    if(currentStudent == nullptr) {
        return;
    }

    //get a folder to export to
    QString filePath = QFileDialog::getExistingDirectory(this,
                                                         tr("Export Files to"),
                                                         "C://",
                                                         QFileDialog::ShowDirsOnly);
    if(filePath.isEmpty()) {
        return;
    }

    //for each student, take data and make output file
    for(GAStudent* student: currentClass->get_students()) {
        currentStudent = student;
        currentAssignmentData = currentStudent->get_data(currentAssignment);
        GAOutputFile *newFile = new GAOutputFile(filePath.toStdString(), currentAssignmentData);
        newFile->open_empty();
    }

    //then close dialog
    close();
}


/*!
 * @brief Slot called when user clicks the Flag button
 */
void GradingSession::on_flagButton_clicked()
{
    //if there is no student or line selected
    if(currentStudent == nullptr || currentRubric->get_ec() == nullptr ||
            ui->codeEdit->get_current_line() == -1) {
        return;
    }

    if(selectedAnnotation == nullptr) {
        return;
    }

    if(check_for_annotation()) {
        return;
    }
    else {
        //make new annotation based on selectedAnnotation
        GAAnnotation* newAnnotation = selectedAnnotation->copy();

        //add location information
        newAnnotation->set_filename(ui->fileList->currentItem()->text().toStdString());
        newAnnotation->set_line(ui->codeEdit->get_current_line());

        //add to assignment data
        currentAssignmentData->add_annotation(newAnnotation);

        ui->searchBox->clear();
        ui->previewEdit->clear();
        ui->annotationList->clear();
        ui->codeEdit->add_annotation();
        print_preview();
    }
}


/*!
 * @brief Slot called when Edit button is clicked
 */
void GradingSession::on_editButton_clicked()
{
    //if current student or annotation is null, or no current line selected,
    //do nothing
    if(currentStudent == nullptr || selectedAnnotation == nullptr ||
            ui->codeEdit->get_current_line() == -1) {
        return;
    }

    if(check_for_annotation()) {
        return;
    }

    //make a new annotation based off this edit
    GAAnnotation* newAnnotation = selectedAnnotation->copy();

    //create a flag dialog based on the selected annotation
    fd = new FlagDialog(this, currentRubric, newAnnotation);
    fd->exec();

    //if the user has pressed cancel, do nothing
    if(fd->get_new_annotation() == nullptr) {
        delete newAnnotation;
        return;
    } else {
        //get the edited annotation
        selectedAnnotation = fd->get_new_annotation();

        //add to assignment data
        currentAssignmentData->add_annotation(selectedAnnotation);

        ui->searchBox->clear();
        ui->previewEdit->clear();
        ui->annotationList->clear();
        ui->codeEdit->add_annotation();
        print_preview();
    }

    delete fd;
}


/*!
 * @brief Slot called when the user clicks the Add button
 */
void GradingSession::on_addNewButton_clicked()
{
    //if current student is null or no current line selected, do nothing
    if(currentStudent == nullptr ||ui->codeEdit->get_current_line() == -1) {
        return;
    }

    if(check_for_annotation()) {
        return;
    }

    //create a flag dialog to make a new annotation
    fd = new FlagDialog(this, currentRubric, 1);
    fd->exec();

    //if the user pressed cancel, do nothing
    if(fd->get_new_annotation() == nullptr) {
        return;
    }
    else {
        //get the new annotation
        GAAnnotation *newAnnotation = fd->get_new_annotation();
        newAnnotation->set_grading_assistant(gradingAssistant);
        newAnnotation->set_assignment_data(currentAssignmentData);
        //add location information
        newAnnotation->set_filename(ui->fileList->currentItem()->text().toStdString());
        newAnnotation->set_line(ui->codeEdit->get_current_line());

        //add to assignment data
        currentAssignmentData->add_annotation(fd->get_new_annotation());

        ui->searchBox->clear();
        ui->previewEdit->clear();
        ui->annotationList->clear();
        ui->codeEdit->add_annotation();
        selectedAnnotation = newAnnotation;
        print_preview();
    }

    delete fd;
}


/*!
 * @brief Slot representing the text in the search box being changed by the user
 * @param arg1 - current text in search box
 */
void GradingSession::on_searchBox_textChanged(const QString &arg1)
{
    //get all annotations fitting search term
    currentAnnotations = gradingAssistant->query_annotation(arg1.toStdString());

    //clear, then refill annotation list
    ui->annotationList->clear();

    for(GAAnnotation* a: currentAnnotations) {
        QListWidgetItem* item = new QListWidgetItem;
        item->setText(QString::fromStdString(a->get_title()));
        ui->annotationList->addItem(item);
    }

    //as long is there is an annotation, automatically select the current row
    if(currentAnnotations.size() > 0) {
        ui->annotationList->setCurrentRow(0);
    }
}


/*!
 * @brief Prints out a preview of the selected annotation
 */
void GradingSession::print_preview()
{
    ui->previewEdit->clear();
    ui->previewEdit->setFontPointSize(14);
    ui->previewEdit->setFontWeight(QFont::Bold);

    bool change = true;

    //check to make sure selectedAnnotation has category that is included in
    //current rubric, if not, change it to one that is
    for(GARubricRow* row: currentRubric->get_rows()) {
        if(selectedAnnotation->get_category() == row->get_category()
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


/*!
 * @brief Slot called when the user clicks on the code edit
 */
void GradingSession::update_selection()
{
    //get the current line from the code edit
    int currentLine = ui->codeEdit->textCursor().blockNumber() + 1;

    //find the annotation matching this line/file pair in the student's data
    GAAnnotation* selected = currentAssignmentData->get_annotation(currentFile, currentLine);

    //if a selection was found
    if(selected != nullptr)
    {
        //set as selected and show in preview edit
        selectedAnnotation = selected;
        print_preview();
    }
    else //nothing should show in code edit
    {
        ui->previewEdit->clear();
    }
}

/*!
 * @brief Context menu shown when user right clicks on code edit
 * @param pos
 */
void GradingSession::show_context_menu(const QPoint &pos)
{
    // Handle global position
    QPoint globalPos = ui->codeEdit->mapToGlobal(pos);

    // Create menu and insert some actions
    QMenu myMenu;
    myMenu.addAction("Remove", this, SLOT(remove_annotation()));
    myMenu.addAction("Add New", this, SLOT(on_addNewButton_clicked()));

    // Show context menu at handling position
    myMenu.exec(globalPos);
}

void GradingSession::remove_annotation() {

    //make sure current line is annotation
    int currentLine = ui->codeEdit->textCursor().blockNumber() + 1;
    selectedAnnotation = currentAssignmentData->get_annotation(currentFile, currentLine);

    if(selectedAnnotation != nullptr) {
        ui->codeEdit->remove_annotation();
        currentAssignmentData->remove_annotation(selectedAnnotation);
        ui->previewEdit->clear();
    }
    else {
        return;
    }
}

bool GradingSession::check_for_annotation() {

    int currentLine = ui->codeEdit->textCursor().blockNumber() + 1;
    GAAnnotation* selected  = currentAssignmentData->get_annotation(currentFile, currentLine);

    if(selected != nullptr) {
        return true;
    }
    else {
        return false;
    }
}
