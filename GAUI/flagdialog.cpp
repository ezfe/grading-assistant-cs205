#include "flagdialog.h"
#include "ui_flagdialog.h"

/*!
 * @brief Sets up a flag dialog to be used to make a new annotation
 * @param parent - GradingSession
 * @param r - current rubric
 * @param i - variable to differentiate making new
 */
FlagDialog::FlagDialog(QWidget *parent, GARubric *r, int i) :
    QDialog(parent),
    ui(new Ui::FlagDialog)
{
    ui->setupUi(this);
    this->setWindowTitle("Add New Annotation");

    rubric = r;
    newAnnotation = nullptr;

    for(GARubricRow *r : rubric->get_rows()) {
        categories.push_back(r->get_category());
    }

    //set up combo box with rubric categories
    for(std::string s : categories)
    {
        ui->categoryBox->addItem(QString::fromStdString(s));
    }

    //set default value for point edit
    ui->pointsEdit->setText(QString::number(0));
}


/*!
 * @brief Sets up a flag dialog to be used to edit an existing annotation
 * @param parent - GradingSession
 * @param r - current rubric
 * @param annotation - annotation to edit
 */
FlagDialog::FlagDialog(QWidget *parent, GARubric *r,
                       GAAnnotation *annotation) : QDialog(parent),
    ui(new Ui::FlagDialog)
{
    ui->setupUi(this);
    this->setWindowTitle("Edit Existing Annotation");

    rubric = r;
    newAnnotation = annotation;

    for(GARubricRow *r : rubric->get_rows()) {
        categories.push_back(r->get_category());
    }

    flagType = annotation->get_type();

    //set up combo box with rubric categories
    for(std::string s : categories)
    {
        ui->categoryBox->addItem(QString::fromStdString(s));
    }

    //set default values for all edits
    update_categories();
    ui->nameEdit->setText(QString::fromStdString(newAnnotation->get_title()));
    ui->pointsEdit->setText(QString::number(newAnnotation->get_points()));
    ui->descriptionEdit->setPlainText(QString::fromStdString(newAnnotation->get_description()));
    ui->categoryBox->setCurrentText(QString::fromStdString(newAnnotation->get_category()));
}


/*!
 * @brief Destructs everything created for this dialog
 */
FlagDialog::~FlagDialog()
{
    delete ui;
}


/*!
 * @brief Gets this dialog's annotation
 * @return newAnnotation
 */
GAAnnotation* FlagDialog::get_new_annotation() {
    return newAnnotation;
}


/*!
 * @brief Updates the combo box based on the current point value
 */
void FlagDialog::update_categories()
{
    //if EC, add that option to combo box and select it
    if(flagType == "GA_ANNOTATION_EXTRACREDIT")
    {
        if(ui->categoryBox->count() == categories.size())
        {
            ui->categoryBox->addItem("Extra Credit");
            ui->categoryBox->setCurrentIndex(categories.size());
        }
    } //remove EC option
    else if(flagType == "GA_ANNOTATION_PROBLEM" || flagType == "GA_ANNOTATION_COMMENT")
    {
        if(ui->categoryBox->count() != categories.size())
        {
            ui->categoryBox->removeItem(categories.size());
        }
    }
    else
    {
        return;
    }
}


/*!
 * @brief Closes the dialog without saving anything
 */
void FlagDialog::on_cancelButton_clicked()
{
    newAnnotation = nullptr;
    close();
}


/*!
 * @brief Slot called when user clicks the Flag button
 */
void FlagDialog::on_flagButton_clicked()
{
    //if the user is making a new annotation, make new
    if(newAnnotation == nullptr) {
        newAnnotation = new GAAnnotation(flagType);
    }
    else //set type of old
    {
        newAnnotation->set_type(flagType);
    }

    //set category
    if(flagType == "GA_ANNOTATION_EXTRACREDIT") {
        newAnnotation->set_category("Extra Credit");
    }
    else {
        newAnnotation->set_category(categories[ui->categoryBox->currentIndex()]);
    }

    //set other fields
    newAnnotation->set_title(ui->nameEdit->text().toStdString());
    newAnnotation->set_points(ui->pointsEdit->text().toInt());
    newAnnotation->set_description(ui->descriptionEdit->toPlainText().toStdString());

    if(ui->uniqueErrorBox->isChecked()) {
        //don't save to database, just for this student's assignment
    }

    close();
}


/*!
 * @brief Slot called whenever the text in the points edit changes
 * @param arg1 - changed text
 */
void FlagDialog::on_pointsEdit_textChanged(const QString &arg1)
{
    //convert text to integer
    int points = arg1.toInt();

    //determine type by point value and delegate appropriately
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
