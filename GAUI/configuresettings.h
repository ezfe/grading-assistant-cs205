#ifndef CONFIGURESETTINGS_H
#define CONFIGURESETTINGS_H

#include <QWidget>
#include <QDialog>

namespace Ui {
class ConfigureSettings;
}

class ConfigureSettings : public QDialog
{
    Q_OBJECT

public:
    explicit ConfigureSettings(QWidget *parent = 0);
    ~ConfigureSettings();

    std::string get_username();
    std::string get_hostname();
    std::string get_path();

private slots:
    void on_startButton_clicked();

private:
    Ui::ConfigureSettings *ui;

    //User information
    std::string username;
    std::string hostname;
    std::string path;
};

#endif // CONFIGURESETTINGS_H
