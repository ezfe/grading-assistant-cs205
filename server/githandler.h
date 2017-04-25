#ifndef GITHANDLER_H
#define GITHANDLER_H

#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <exception>
#include <string>
#include <ctime>

#include <QDir>
#include <QString>

#include "unistd.h"
#include <../grading-assistant/filemanager.h>
#include <../grading-assistant/usersettings.h>
#include <../grading-assistant/platform.h>

// Change Directory Defines
#ifdef GA_APPLE_ACTIVE
    #define cd chdir
#elif GA_WINDOWS_ACTIVE
    #define cd _chdir
    #include "direct.h"
#elif GA_LINUX_ACTIVE
    #define cd chdir
#else
    #define cd chdir
#endif

class GitHandler
{
public:
    // Constructor and Destructor for object instantiation
    GitHandler(std::string user, std::string host, std::string remotePath);
    ~GitHandler();

    // Used to alert if current system is recognized
    bool system_recognized(void);

    // Get/Set methods specifying Git Repository local/remote location/path
    void        set_remote_url(const std::string loc);
    std::string get_remote_url(void);

    void        set_remote_path(const std::string path);
    std::string get_remote_path(void);

    void        set_repo_loc(const std::string loc);
    std::string get_repo_loc(void);

    int  get_errors(void);
    void clear_errors(void);

    // Methods used for general interaction with server
    void setup(void);
    void sync(void);
    int  resolve(void);

    // Methods used for addition conflict resolution
    void remove_local(void);

private:

    GitHandler();

    // Boolean values to track object execution state
    bool recsys;
    bool remotefail;
    bool pullfail;
    bool pushfail;

    // Variables used to store Git Repository location information
    std::string remoteURL;
    std::string remotePath;
    std::string localPath;

    // Helper functions used to initialize remote + local Git Repositories
    int make_remote(void);
    int init_repo(void);

    // Helper functions used to sync. local Git Repository
    int load_repo(bool errorcheck = true);
    int save_repo(bool errorcheck = true);

    // Helper functions used throughout object functions
    void        change_dir(const std::string path);
    std::string exec_cmd(const std::string cmd);
    int get_time_stamp(void);
};

#endif // GITHANDLER_H
