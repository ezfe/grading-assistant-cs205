#include "configuresettings.h"
#include "ui_configuresettings.h"


//CONTRUCTOR/DESTRUCTOR


/*!
 * @brief Sets up the configure settings dialog
 * @param parent - BaseScreen
 * @param username - default username
 * @param hostname - default hostname
 * @param path - default path
 */
ConfigureSettings::ConfigureSettings(QWidget *parent, std::string username, std::string hostname,
                                     std::string path) :
    QDialog(parent),
    ui(new Ui::ConfigureSettings)
{
    ui->setupUi(this);

    //set default text
    ui->usernameEdit->setText(QString::fromStdString(username));
    ui->hostnameEdit->setText(QString::fromStdString(hostname));
    ui->pathEdit->setText(QString::fromStdString(path));
}

/*!
 * @brief Destructs everything created for this dialog
 */
ConfigureSettings::~ConfigureSettings()
{
    delete ui;
}


//KEEP TRACK OF USER ACTIONS


/*!
 * @brief Handles what happens when the user clicks the start button
 */
void ConfigureSettings::on_startButton_clicked()
{
    //make sure all text fields are filled in
    if(ui->usernameEdit->text().isEmpty() || ui->hostnameEdit->text().isEmpty() ||
            ui->pathEdit->text().isEmpty()) {
        return;
    }
    else { //if they are, save those values and close
        username = ui->usernameEdit->text().toStdString();
        hostname = ui->hostnameEdit->text().toStdString();
        path = ui->pathEdit->text().toStdString();

        close();
    }
}


//GET INPUTTED INFORMATION


/*!
 * @brief Gets the user's username
 * @return username
 */
std::string ConfigureSettings::get_username() {
    return username;
}

/*!
 * @brief Gets the user's hostname
 * @return hostname
 */
std::string ConfigureSettings::get_hostname() {
    return hostname;
}

/*!
 * @brief Gets the user's path
 * @return path
 */
std::string ConfigureSettings::get_path() {
    return path;
}
