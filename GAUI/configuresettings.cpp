#include "configuresettings.h"
#include "ui_configuresettings.h"

ConfigureSettings::ConfigureSettings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfigureSettings)
{
    ui->setupUi(this);
}

ConfigureSettings::~ConfigureSettings()
{
    delete ui;
}

void ConfigureSettings::on_startButton_clicked()
{
    if(ui->usernameEdit->text().isEmpty() || ui->hostnameEdit->text().isEmpty() ||
            ui->pathEdit->text().isEmpty()) {
        return;
    }
    else {
        username = ui->usernameEdit->text().toStdString();
        hostname = ui->hostnameEdit->text().toStdString();
        path = ui->pathEdit->text().toStdString();

        close();
    }
}

std::string ConfigureSettings::get_username() {
    return username;
}

std::string ConfigureSettings::get_hostname() {
    return hostname;
}

std::string ConfigureSettings::get_path() {
    return path;
}
