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

int GitHandler::make_remote(void)
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

int GitHandler::init_repo(void)
{
    std::string cmd;

    chdir(FileManager::get_app_directory().c_str());

    std::string testgit;

    testgit.append(exec_cmd("ls -ad .git"));

    if(!testgit.compare(""))
    {
        // Initialize the repo
        cmd = "git init";
        exec_cmd(cmd);

        // Add everything in the directory to the repo
        cmd = "git add .";
        exec_cmd(cmd);

        // Record the initial commit + message
        cmd = "git commit -m \" initial commit ";
        cmd.append(std::to_string(get_time_stamp()));
        cmd += "\"";
        exec_cmd(cmd);

        // Add the remote - first check if exists
        std::string testremote;
        testremote.append(exec_cmd("git remote -v"));

        // If doesn't exist - add it
        if(!testremote.compare(""))
        {
            cmd = "git remote add origin ssh://";
            cmd += remoteloc + ":/";
            cmd += remotepath;
            cmd += reponame;
            exec_cmd(cmd);
        }

        // Push to remote
        cmd = "git push origin master";
        exec_cmd(cmd);
    }

    return 0;
}

int GitHandler::load_repo(void)
{

}

int GitHandler::save(void)
{
    return 0;
}

int GitHandler::remove(void)
{
    return 0;
}

std::string GitHandler::exec_cmd(const std::string cmd)
{
    std::string rtn;
    char buff[PATH_MAX];
    FILE* stream = popen(cmd.c_str(), "r");

    if(stream)
    {
        while(fgets(buff, PATH_MAX, stream) != NULL)
        {
            rtn.append(buff);
        }
    }
    else {
        return NULL;
    }

    int status = pclose(stream);

    if(status == -1)
    {
        return NULL;
    }

    if(rtn.size() > 0) rtn.pop_back();

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
