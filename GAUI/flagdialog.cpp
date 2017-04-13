#include "flagdialog.h"
#include "ui_flagdialog.h"

FlagDialog::FlagDialog(QWidget *parent, GradingAssistant *g, GARubric *r,
                       std::string type) :
    QDialog(parent),
    ui(new Ui::FlagDialog)
{
    ui->setupUi(this);
    set_top_enabled();
    ui->chooseFromExistingButton->setChecked(true);

    ga = g;
    rubric = r;
    flagType = type;
    newAnnotation = nullptr;
    selectedAnnotation = nullptr;

    if(type == "GA_ANNOTATION_PROBLEM") {
        this->setWindowTitle("Flag Problem");
        for(GARubricRow *r : rubric->get_rows()) {
            categories.push_back(r->get_category());
        }
        ui->pointsLabel1->setText("Associated Points:");
        ui->pointsLabel2->setText("Associated Points:");

    } else if(type == "GA_ANNOTATION_COMMENT") {
        this->setWindowTitle("Flag Comment");
        for(GARubricRow *r : rubric->get_rows()) {
            categories.push_back(r->get_category());
        }
        ui->pointsEdit1->hide();
        ui->pointsEdit2->hide();
        disable_points();
    }
    else {
        this->setWindowTitle("Flag Extra Credit");
        categories.push_back("Extra Credit");
        ui->pointsLabel1->setText("Associated Points:");
        ui->pointsLabel2->setText("Associated Points:");
    }


    for(std::string s : categories) {
        ui->categoryBox1->addItem(QString::fromStdString(s), 0);
        ui->categoryBox2->addItem(QString::fromStdString(s), 0);
    }
}

FlagDialog::~FlagDialog()
{
    delete ui;
}

void FlagDialog::on_chooseFromExistingButton_clicked()
{
    set_top_enabled();
}

void FlagDialog::on_addNewButton_clicked()
{
    set_bottom_enabled();
}

void FlagDialog::set_top_enabled() {

    ui->searchBox->setEnabled(true);
    ui->annotationList->setEnabled(true);
    ui->nameLabel1->setEnabled(true);
    ui->nameEdit1->setEnabled(true);
    ui->categoryLabel1->setEnabled(true);
    ui->categoryBox1->setEnabled(true);
    ui->pointsLabel1->setEnabled(true);
    ui->pointsEdit1->setEnabled(true);
    ui->descriptionLabel1->setEnabled(true);
    ui->descriptionEdit1->setEnabled(true);

    ui->nameLabel2->setDisabled(true);
    ui->nameEdit2->setDisabled(true);
    ui->categoryLabel2->setDisabled(true);
    ui->categoryBox2->setDisabled(true);
    ui->pointsLabel2->setDisabled(true);
    ui->pointsEdit2->setDisabled(true);
    ui->descriptionLabel2->setDisabled(true);
    ui->descriptionEdit2->setDisabled(true);
    ui->uniqueErrorBox->setDisabled(true);

    if(flagType == "GA_ANNOTATION_COMMENT") {
        disable_points();
    }
}

void FlagDialog::set_bottom_enabled() {

    ui->nameLabel2->setEnabled(true);
    ui->nameEdit2->setEnabled(true);
    ui->categoryLabel2->setEnabled(true);
    ui->categoryBox2->setEnabled(true);
    ui->pointsLabel2->setEnabled(true);
    ui->pointsEdit2->setEnabled(true);
    ui->descriptionLabel2->setEnabled(true);
    ui->descriptionEdit2->setEnabled(true);
    ui->uniqueErrorBox->setEnabled(true);

    ui->searchBox->setDisabled(true);
    ui->annotationList->setDisabled(true);
    ui->nameLabel1->setDisabled(true);
    ui->nameEdit1->setDisabled(true);
    ui->categoryLabel1->setDisabled(true);
    ui->categoryBox1->setDisabled(true);
    ui->pointsLabel1->setDisabled(true);
    ui->pointsEdit1->setDisabled(true);
    ui->descriptionLabel1->setDisabled(true);
    ui->descriptionEdit1->setDisabled(true);

    if(flagType == "GA_ANNOTATION_COMMENT") {
        disable_points();
    }
}

void FlagDialog::on_searchBox_editingFinished()
{
    currentAnnotations = ga->query_annotation(
                ui->searchBox->text().toStdString());

    ui->annotationList->clear();

    for(GAAnnotation* a: currentAnnotations) {
        QListWidgetItem* item = new QListWidgetItem;
        item->setText(QString::fromStdString(a->get_title()));
        ui->annotationList->addItem(item);
    }
}

void FlagDialog::on_annotationList_currentRowChanged(int currentRow)
{
    if(currentRow >= 0) {
        selectedAnnotation = currentAnnotations[currentRow];

        ui->nameEdit1->setText(QString::fromStdString(
                                          selectedAnnotation->get_title()));
        ui->pointsEdit1->setText(QString::number(
                                          selectedAnnotation->get_points()));
        ui->descriptionEdit1->setPlainText(QString::fromStdString(
                                          selectedAnnotation->get_description()));

        for(int i = 0; i < categories.size(); i++) {
            if(categories[i] == selectedAnnotation->get_category()) {
                ui->categoryBox1->setCurrentIndex(i);
            }
        }
    }
}

void FlagDialog::on_cancelButton_clicked()
{
    close();
}

void FlagDialog::on_flagButton_clicked()
{
    if(ui->addNewButton->isChecked()) {
        newAnnotation = new GAAnnotation(flagType);
        newAnnotation->set_category(categories[ui->categoryBox2->currentIndex()]);
        newAnnotation->set_title(ui->nameEdit2->text().toStdString());
        if(flagType != "GA_ANNOTATION_COMMENT") {
            newAnnotation->set_points(ui->pointsEdit2->text().toInt());
        }
        newAnnotation->set_description(ui->descriptionEdit2->toPlainText().toStdString());

        if(ui->uniqueErrorBox->isChecked()) {
            //don't save to database, just for this student's assignment
        }
    }
    else {
        if(selectedAnnotation == nullptr) {
            close();
        }
        selectedAnnotation->set_type(flagType);
        selectedAnnotation->set_title(ui->nameEdit1->text().toStdString());
        selectedAnnotation->set_category(categories[ui->categoryBox1->currentIndex()]);
        if(flagType != "GA_ANNOTATION_COMMENT") {
            selectedAnnotation->set_points(ui->pointsEdit1->text().toInt());
        }
        selectedAnnotation->set_description(ui->descriptionEdit1->toPlainText().toStdString());

        newAnnotation = selectedAnnotation;
    }

    close();
}

GAAnnotation* FlagDialog::get_new_annotation() {
    return newAnnotation;
}

void FlagDialog::disable_points() {
    ui->pointsEdit1->setDisabled(true);
    ui->pointsEdit2->setDisabled(true);
    ui->pointsLabel1->setDisabled(true);
    ui->pointsLabel2->setDisabled(true);
}
