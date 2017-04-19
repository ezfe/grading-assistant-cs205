#include "flagdialog.h"
#include "ui_flagdialog.h"

FlagDialog::FlagDialog(QWidget *parent, GradingAssistant *g, GARubric *r, int i) :
    QDialog(parent),
    ui(new Ui::FlagDialog)
{
    ui->setupUi(this);
    this->setWindowTitle("Add New Annotation");

    ga = g;
    rubric = r;
    newAnnotation = nullptr;
    flagType = "";

    for(GARubricRow *r : rubric->get_rows()) {
        categories.push_back(r->get_category());
    }

    update_categories();
    ui->pointsEdit->setText(QString::number(0));
}

FlagDialog::FlagDialog(QWidget *parent, GradingAssistant *g, GARubric *r,
                       GAAnnotation *annotation) : QDialog(parent),
    ui(new Ui::FlagDialog)
{
    ui->setupUi(this);
    this->setWindowTitle("Edit Existing Annotation");

    ga = g;
    rubric = r;
    newAnnotation = annotation;

    for(GARubricRow *r : rubric->get_rows()) {
        categories.push_back(r->get_category());
    }

    flagType = "";

    update_categories();

    flagType = annotation->get_type();

    ui->nameEdit->setText(QString::fromStdString(newAnnotation->get_title()));
    ui->pointsEdit->setText(QString::number(newAnnotation->get_points()));
    ui->descriptionEdit->setPlainText(QString::fromStdString(newAnnotation->get_description()));
    ui->categoryBox->setCurrentText(QString::fromStdString(newAnnotation->get_category()));
}

FlagDialog::~FlagDialog()
{
    delete ui;
}

void FlagDialog::on_cancelButton_clicked()
{
    close();
}

void FlagDialog::on_flagButton_clicked()
{
    if(newAnnotation == nullptr) {
        newAnnotation = new GAAnnotation(flagType);
    }

    if(flagType == "GA_ANNOTATION_EXTRACREDIT") {
        newAnnotation->set_category("Extra Credit");
    }
    else {
        newAnnotation->set_category(categories[ui->categoryBox->currentIndex()]);
    }

    newAnnotation->set_title(ui->nameEdit->text().toStdString());
    newAnnotation->set_points(ui->pointsEdit->text().toInt());
    newAnnotation->set_description(ui->descriptionEdit->toPlainText().toStdString());

    if(ui->uniqueErrorBox->isChecked()) {
        //don't save to database, just for this student's assignment
    }

    close();
}

GAAnnotation* FlagDialog::get_new_annotation() {
    return newAnnotation;
}

void FlagDialog::on_pointsEdit_textChanged(const QString &arg1)
{
    int points = arg1.toInt();

    if(points > 0)
    {
        flagType = "GA_ANNOTATION_EXTRACREDIT";
        update_categories();
    }
    else if(points < 0)
    {
        flagType = "GA_ANNOTATION_PROBLEM";
        update_categories();
    }
    else
    {
        flagType = "GA_ANNOTATION_COMMENT";
        update_categories();
    }
}

void FlagDialog::update_categories()
{
    if(flagType == "GA_ANNOTATION_EXTRACREDIT")
    {
        if(ui->categoryBox->count() == categories.size())
        {
            ui->categoryBox->addItem("Extra Credit");
            ui->categoryBox->setCurrentIndex(categories.size());
        }
    }
    else if(flagType == "GA_ANNOTATION_PROBLEM" || flagType == "GA_ANNOTATION_COMMENT")
    {
        if(ui->categoryBox->count() != categories.size())
        {
            ui->categoryBox->removeItem(categories.size());
        }
    }
    else
    {
        for(std::string s : categories)
        {
            ui->categoryBox->addItem(QString::fromStdString(s));
        }
    }
}

