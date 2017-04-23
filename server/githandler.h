#ifndef GITHANDLER_H
#define GITHANDLER_H

#include <iostream>
#include <sstream>
#include <iomanip>
#include <exception>
#include <string>

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
    GitHandler(std::string user, std::string host, std::string path, std::string name);
    ~GitHandler();

    bool system_recognized(void);

    void set_remote_loc(const std::string loc);
    std::string get_remote_loc(void);

    void set_remote_path(const std::string path);
    std::string get_remote_path(void);

    void set_repo_loc(const std::string loc);
    std::string get_repo_loc(void);

    void set_repo_name(const std::string name);
    std::string get_repo_name(void);

    int get_errors(void);
    void clear_errors(void);

    void setup(void);

    void sync(void);

    void resolve(void);

    int remove_local(void);

    int remove_remote(void);

    std::string exec_cmd(const std::string cmd);

private:

    bool recsys;
    bool remotefail;
    bool pullfail;
    bool pushfail;

    std::string remoteloc;
    std::string remotepath;

    std::string repoloc;
    std::string reponame;

    int make_remote(void);

    int init_repo(void);

    int load_repo(void);

    int save_repo(void);

    void change_dir(const std::string path);

    //std::string exec_cmd(const std::string cmd);

    std::string partial_string(const std::string orig, const int numwords);

    int get_time_stamp(void);
};

#endif // GITHANDLER_H
