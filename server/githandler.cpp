#include "githandler.h"

GitHandler::GitHandler()
{
    remoteloc  = "spr2017_l2g4@139.147.9.185";
    remotepath = "home/spr2017_l2g4/";

    repoloc    = "\"" + FileManager::get_app_directory() + "\"";
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
    repoloc = path;
}

std::string GitHandler::get_remote_loc()
{
    return repoloc;
}

void GitHandler::set_repo_name(std::string name)
{
    reponame = name;
}

std::string GitHandler::get_repo_name()
{
    return reponame;
}

int GitHandler::make_remote(void)
{
    std::string command;

    command += "ssh " + remoteloc;
    command += " \"git init --bare --shared ";
    command += reponame + "\"";

    if(exec_cmd(command) != "error") return 1;
    else return -1;
 }

int GitHandler::make_remote_clean(void)
{
    return 0;
}

// Adds checks to make sure commands and their returns are sensible
int GitHandler::init_repo(void)
{
    std::string cmd;

    chdir(FileManager::get_app_directory().c_str());

    cmd = "git init";
    std::cout << cmd << exec_cmd(cmd) << std::endl;

    cmd = "git add .";
    std::cout << cmd << exec_cmd(cmd) << std::endl;

    cmd = "git commit -m \" initial commit ";
    cmd.append(std::to_string(get_time_stamp()));
    cmd += "\"";
    std::cout << cmd << exec_cmd(cmd) << std::endl;

    cmd = "git remote add origin ssh://";
    cmd += remoteloc + ":/";
    cmd += remotepath;
    cmd += reponame;
    std::cout << cmd << exec_cmd(cmd) << std::endl;

    cmd = "git push origin master";
    std::cout << cmd << exec_cmd(cmd) << std::endl;

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

std::string GitHandler::exec_cmd(std::string cmd)
{
    std::string rtn;
    std::cout << rtn << std::endl;
    char buff[PATH_MAX];
    FILE* stream = popen(cmd.c_str(), "r");

    if(stream)
    {
        while(fgets(buff, PATH_MAX, stream) != NULL)
        {
            rtn.append(buff);
        }

    }
    else return "error";

    int status = pclose(stream);

    if(status == -1)
    {
        return "error";

    }
    rtn.pop_back();
    return rtn;
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
