#ifndef GITHANDLER_H
#define GITHANDLER_H

#include <iostream>
#include <../grading-assistant/filemanager.h>
#include <../grading-assistant/usersettings.h>
#include <../grading-assistant/platform.h>


class GitHandler
{
public:
    GitHandler();
    ~GitHandler();

    static void set_repo_loc(std::string path); // - Could use UserSettings?
    static std::string get_repo_loc();

    static void set_repo_name(std::string name);
    static std::string get_repo_name();

    static int init_repo(void);

    static int save_db(void);

    static int remove_db(void);

private:

    /*!
     * \brief store_commit_val
     * Stores commit value after push. Later used to determine if
     * branch is furthest ahead.
     *
     * \param val
     */
    void store_commit_val(std::string val);



    std::string repoloc;
    std::string reponame;
    std::string currcommit;

};

#endif // GITHANDLER_H
