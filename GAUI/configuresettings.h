#ifndef CONFIGURESETTINGS_H
#define CONFIGURESETTINGS_H

#include <QWidget>
#include <QDialog>

namespace Ui {
class ConfigureSettings;
}

/*!
 * @brief This is a dialog that allows the users to configure basic program settings
 */
class ConfigureSettings : public QDialog
{
    Q_OBJECT

public:

    //Constructor/Destructor
    explicit ConfigureSettings(QWidget *parent = 0, std::string username = 0, std::string hostname
                               = 0, std::string path = 0);
    ~ConfigureSettings();

    //Get inputted information
    std::string get_username();

    std::string get_hostname();

    std::string get_path();

    bool is_offline() { return this->offline; }
private slots:

    //Keep track of user actions
    void on_startButton_clicked();

    void on_startOffline_clicked();

private:
    Ui::ConfigureSettings *ui;

    //User information
    std::string username;
    std::string hostname;
    std::string path;
    bool offline = false;
};

#endif // CONFIGURESETTINGS_H
