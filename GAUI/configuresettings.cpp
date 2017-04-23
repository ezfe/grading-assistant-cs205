#include "configuresettings.h"
#include "ui_configuresettings.h"

/*!
 * @brief Sets up the configure settings dialog
 * @param parent - BaseScreen
 */
ConfigureSettings::ConfigureSettings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfigureSettings)
{
    ui->setupUi(this);
}

/*!
 * @brief Destructs everything created for this dialog
 */
ConfigureSettings::~ConfigureSettings()
{
    delete ui;
}

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
