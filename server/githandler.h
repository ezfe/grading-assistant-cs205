#ifndef GITHANDLER_H
#define GITHANDLER_H

#include <iostream>
#include "unistd.h"
#include <iomanip>
#include <../grading-assistant/filemanager.h>
#include <../grading-assistant/usersettings.h>
#include <../grading-assistant/platform.h>


class GitHandler
{
public:
    GitHandler();
    ~GitHandler();

    void set_remote_loc(std::string path);
    std::string get_remote_loc();

    void set_repo_name(std::string name);
    std::string get_repo_name();

    int make_remote(void);

    int make_remote_clean(void);

    int init_repo(void);

    int clone_repo(void);

    int save_db(void);

    int remove_db(void);

private:

    std::string exec_cmd(std::string cmd);

    /*!
     * \brief store_commit_val
     * Stores commit value after push. Later used to determine if
     * branch is furthest ahead.
     *
     * \param val
     */
    void store_commit_val(std::string val);

    bool remote_exists(void);

    int get_time_stamp(void);

    std::string remoteloc;
    std::string remotepath;

    std::string repoloc;
    std::string reponame;

    std::string currcommit;

};

#endif // GITHANDLER_H
