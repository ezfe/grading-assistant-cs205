#include "githandler.h"

GitHandler::GitHandler()
{
    remoteloc  = "spr2017_l2g4@139.147.9.185";
    std::cout << remoteloc << std::endl;
    repoloc    = "\"" + FileManager::get_app_directory() + "\"";
    std::cout << repoloc << std::endl;
    reponame   = "repo_server.git";
    currcommit = "";
}

GitHandler::~GitHandler(){}

/* First returned line type when valid add & commit
[master a4785e2] now added file
*/

/* Return when nothing has been added
On branch master
Your branch is up-to-date with 'origin/master'.
Untracked files:
*/
/*!
 * \brief GitHandler::set_repo_loc
 * \param path The path of repo location
 *  Must be in format: user@location:/path/to/repo
 */
void GitHandler::set_remote_loc(std::string path)
{
    repoloc.clear();
    repoloc += "ssh://" + path;
}

std::string GitHandler::get_remote_loc()
{
    return repoloc;
}

void GitHandler::set_repo_name(std::string name)
{
    reponame.clear();
    reponame += name + ".git";
}

std::string GitHandler::get_repo_name()
{
    return reponame;
}

int GitHandler::make_remote(void)
{
    std::string command;
    command += remoteloc;
    command += " \"git init --bare --shared repo_test_";
    command += std::to_string(get_time_stamp());
    command += ".git\"";

    std::cout << command << std::endl;
    system(command.c_str());

 }

int GitHandler::make_remote_clean(void)
{

}

int GitHandler::init_repo(void)
{

}

int GitHandler::clone_repo(void)
{

}

int GitHandler::save_db(void)
{

}

int GitHandler::remove_db(void)
{

}

int GitHandler::get_time_stamp(void)
{
    struct tm tm;
    time_t t;
    time(&t);
    std::string tm_val = ctime(&t);
    strptime(tm_val.c_str(), "%c", &tm);
    t = mktime(&tm);
    std::cout << t << std::endl;
    return (int) t;
}
