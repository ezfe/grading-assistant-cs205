#include "githandler.h"

// Note: Need to look into making existing dir. git repo, then pulling from remote

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
    this->repoloc = path;
}

std::string GitHandler::get_remote_loc()
{
    return repoloc;
}

void GitHandler::set_repo_name(std::string name)
{
    this->reponame = name;
}

std::string GitHandler::get_repo_name()
{
    return reponame;
}

int GitHandler::make_remote(void) // Add checks for if remote already exists (although recreating not necessarily bad)
{
    std::string command;

    command += "ssh " + remoteloc;
    command += " \"git init --bare --shared ";
    command += reponame + "\"";

    try{
        exec_cmd(command);
    }
    catch(std::runtime_error &e)
    {
        return -1;
        std::cerr << e.what() << std::endl;
    }
    return 0;
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

    // Initialize the repo
    cmd = "git init";

    if((partial_string(exec_cmd(cmd), 1) != "Initialized") ||
       (partial_string(exec_cmd(cmd), 1) != "Reinitialized"))
    {
        std::cerr << "Something wrong with \"git init\"" << std::endl;
        return -1;
    }

    // Add everything in the directory to the repo
    cmd = "git add .";

    if(exec_cmd(cmd) != "")
    {
        std::cerr << "Somethhing went wrong with \"git add\"";
        return -2;
    }

    // Record the initial commit + message                     <==== Add fail recognition here
    cmd = "git commit -m \" initial commit ";
    cmd.append(std::to_string(get_time_stamp()));
    cmd += "\"";

    if(partial_string(exec_cmd(cmd), 2) != "stuff")            // Fix this conditional
    {
        std::cerr << "Something went wrong with \"git commit ...\"";
        return -3;
    }

    // Add the remote
    cmd = "git remote add origin ssh://";
    cmd += remoteloc + ":/";
    cmd += remotepath;
    cmd += reponame;
    if(partial_string(exec_cmd(cmd), 2) != "stuff")
    {
        std::cerr << "Something went wrong with \" git remote add ... \"";
        return -4;
    }

    // Push to remote
    cmd = "git push origin master";
    if(partial_string(exec_cmd(cmd), 2) != "stuff")
    {
        std::cerr << "Something went wrong with \"git push ...\"";
        return -5;
    }

    return 0;
}

int GitHandler::clone_repo(void)
{
    std::string cmd;

    cmd = "git clone ssh://";
    cmd += remoteloc + ":/";
    cmd += remotepath;
    cmd += reponame;

    std::cout << cmd << std::endl;
    return 0;
}

int GitHandler::save_db(void)
{
    return 0;
}

int GitHandler::remove_db(void)
{
    return 0;
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
    else return NULL;

    int status = pclose(stream);

    if(status == -1)
    {
        return NULL;

    }
    rtn.pop_back();
    return rtn;
}

std::string GitHandler::partial_string(std::string orig, int numwords)
{
    std::string rtnstring;
    std::string buff;
    std::stringstream ss(orig);
    for(int i = 0; i < numwords; i++) {
        ss >> buff;
        rtnstring += buff;
    }
    return rtnstring;
}

int GitHandler::get_time_stamp(void)
{
    struct tm tm;
    time_t t;
    time(&t);
    std::string tm_val = ctime(&t);
    strptime(tm_val.c_str(), "%c", &tm);
    t = mktime(&tm);
    return (int) t;
}
