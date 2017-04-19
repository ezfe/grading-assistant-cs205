#ifndef GITHANDLER_H
#define GITHANDLER_H

#include <iostream>
#include <sstream>
#include <iomanip>
#include <exception>

#include "unistd.h"

#include <../grading-assistant/filemanager.h>
#include <../grading-assistant/usersettings.h>
#include <../grading-assistant/platform.h>


class GitHandler
{
public:
    GitHandler();
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

    int make_remote(void);

    int init_repo(void);

    int load_repo(void);

    int save_repo(void);

    int remove_local(void);

    int remove_remote(void);

private:

    bool recsys;

    std::string remoteloc;
    std::string remotepath;

    std::string repoloc;
    std::string reponame;

    std::string exec_cmd(const std::string cmd);

    std::string partial_string(std::string orig, int numwords);

    int get_time_stamp(void);
};

#endif // GITHANDLER_H
