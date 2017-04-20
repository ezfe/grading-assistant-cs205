#include "githandler.h"

/*!
 * \brief GitHandler::GitHandler
 *
 * Constructor for GitHandler object.
 *
 * Once created, be sure to check the system is recognized (Apple/Linux).
 *
 * Sets default values for remote location, remote path, and repo name as follows:
 *
 * spr2017_l2g4@139.147.9.185
 * home/spr2017_l2g4/
 * repo_server.git
 *
 * Assumes user has set up SSH keys for remote location.
 *
 */
GitHandler::GitHandler()
{
    // Recognize if we will be able to operate on this system.
    if ((GA_PLATFORM == GA_PLATFORM_APPLE) || (GA_PLATFORM == GA_PLATFORM_LINUX))
    {
        recsys = true;
    }
    else recsys = false;

    // Ensure that our (possibly future) repo exists
    FileManager::assure_directory_exists(FileManager::get_app_directory());

    // Default values for repo location
    remoteloc  = "spr2017_l2g4@139.147.9.185";
    remotepath = "home/spr2017_l2g4/";

    repoloc    = "\"" + FileManager::get_app_directory() + "\"";
    reponame   = "repo_server.git";
}

GitHandler::~GitHandler(){}

/*!
 * \brief GitHandler::system_recognized
 *
 * This method returns if the system being used is recognized.
 *
 * Currently supports Apple and Linux platforms.
 *
 * \return bool If the system is recognized or not.
 */
bool GitHandler::system_recognized(void)
{
    return this->recsys;
}

/*!
 * \brief GitHandler::set_remote_loc
 *
 * The method sets the remote Git Repository location
 *
 * Must be in the format: user@location
 * e.g. user05@123.456.7.890
 *
 * \param loc The location of the Git Repository
 */
void GitHandler::set_remote_loc(const std::string loc)
{
    this->remoteloc = loc;
}


/*!
 * \brief GitHandler::get_remote_loc
 *
 * This method returns the remote location of the Git Repository
 *
 * \return std::string The remote location of the Git Repository
 */
std::string GitHandler::get_remote_loc()
{
    return this->remoteloc;
}

/*!
 * \brief GitHandler::set_remote_path
 *
 * This method sets the remote path of the Git Repository
 *
 * Must be in the format: path/to/location/
 * e.g. home/user05/
 *
 * \param path The user-defined path to Git Repository will reside
 */
void GitHandler::set_remote_path(const std::string path)
{
    this->remotepath = path;
}

/*!
 * \brief GitHandler::get_remote_path
 *
 * This method returns the remote path of the Git Repository
 *
 * \return std::string The user-defined path of the Git Repository
 */
std::string GitHandler::get_remote_path()
{
    return this->remotepath;
}

void GitHandler::set_repo_loc(const std::string loc)
{
    this->repoloc = loc;
}

std::string GitHandler::get_repo_loc(void)
{
    return this->repoloc;
}

void GitHandler::set_repo_name(const std::string name)
{
    this->reponame = name;
}

std::string GitHandler::get_repo_name()
{
    return this->reponame;
}

int GitHandler::make_remote(void)
{
    if(!system_recognized()) return -1;

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
        std::cerr << "In make_remote(): " << e.what() << std::endl;
    }
    return 0;
}

int GitHandler::init_repo(void)
{
    if(!system_recognized()) return -1;

    try
    {
        std::string cmd;
        if((GA_PLATFORM == GA_PLATFORM_APPLE) || (GA_PLATFORM == GA_PLATFORM_LINUX))
        {
            chdir(FileManager::get_app_directory().c_str());
        }
        else if(GA_PLATFORM == GA_PLATFORM_WINDOWS)
        {
            _chdir(FileManager::get_app_directory().c_str());
        }

        std::string testgit;

        testgit.append(exec_cmd("ls -ad .git"));

        if(!testgit.compare(""))
        {
            // Initialize the repo
            cmd = "git init";
            exec_cmd(cmd);

            time_t t;
            time(&t);
            std::string tm_val = ctime(&t);
            cmd = "echo \"Git Repository created: ";
            cmd += tm_val;
            cmd += " \" >> INITLOG.txt";
            exec_cmd(cmd);

            // Add everything in the directory to the repo
            cmd = "git add .";
            exec_cmd(cmd);

            // Record the initial commit + message
            cmd = "git commit -m \" initial commit ";
            cmd.append(std::to_string(get_time_stamp()));
            cmd += "\"";
            exec_cmd(cmd);
        }

        // Add the remote - first check if exists
        std::string testremote;
        testremote.append(exec_cmd("git remote -v"));

        // If doesn't exist - add it
        if(testremote == "")
        {
            cmd = "git remote add origin ssh://";
            cmd += remoteloc + ":/";
            cmd += remotepath;
            cmd += reponame;
            exec_cmd(cmd);
        }

        // Push to remote - first check if necessary
        std::string testpush;
        testpush.append(exec_cmd("git status"));

        if(testpush.compare("On branch master\n"
                            "Your branch is up-to-date with 'origin/master'.\n"
                            "nothing to commit, working tree clean"))
        {
            // === This ignores a delimiter
            cmd = "git fetch";
            exec_cmd(cmd);

            cmd = "git rebase origin/master";
            exec_cmd(cmd);

            cmd = "git rebase --continue";
            exec_cmd(cmd);

            cmd = "git rebase --skip";
            exec_cmd(cmd);
            // === End ignore

            cmd = "git push --set-upstream origin master";
            exec_cmd(cmd);
        }
    }
    catch(std::runtime_error &e)
    {
        return -1;
        std::cerr << "In init_repo(): " << e.what() << std::endl;
    }
    return 0;
}

int GitHandler::load_repo(void)
{
    if(!system_recognized()) return -1;

    try
    {

        chdir(FileManager::get_app_directory().c_str());

        std::string testfetch;
        testfetch.append(exec_cmd("git fetch"));
        if(!testfetch.compare(""))
        {
            exec_cmd("git merge FETCH_HEAD");
        }
    }
    catch(std::runtime_error &e)
    {
        return -1;
        std::cerr << "In load_repo(): " << e.what() << std::endl;
    }
    return 0;
}

int GitHandler::save_repo(void)
{
    if(!system_recognized()) return -1;

    try
    {

        chdir(FileManager::get_app_directory().c_str());

        std::string teststatus;
        std::string cmd;

        teststatus.append(exec_cmd("git status"));

        if(teststatus.compare("On branch master\n"
                              "Your branch is up-to-date with 'origin/master'.\n"
                              "nothing to commit, working tree clean"))
        {
            cmd = "git add .";
            exec_cmd(cmd);

            cmd = "git commit -m \"";
            cmd.append(std::to_string((get_time_stamp())));
            cmd += "\"";
            exec_cmd(cmd);

            cmd = "git push";
            exec_cmd(cmd);
        }
    }
    catch(std::runtime_error &e)
    {
        return -1;
        std::cerr << "In save_repo(): " << e.what() << std::endl;
    }
    return 0;
}

int GitHandler::remove_local(void)
{
    if(!system_recognized()) return -1;

    try
    {
        std::string cmd;

        cmd = "rm -rf ";
        cmd.append(FileManager::get_app_directory());

        exec_cmd(cmd);
    }
    catch(std::runtime_error &e)
    {
        return -1;
        std::cerr << "In remove_local(): " << e.what() << std::endl;
    }
    return 0;
}

int GitHandler::remove_remote(void)
{
    if(!system_recognized()) return -1;

    try
    {
        std::string cmd;

        cmd = "ssh ";
        cmd += remoteloc;

        cmd += " \"rm -rf ";
        cmd += reponame;
        cmd += "\"";

        exec_cmd(cmd);
    }
    catch(std::runtime_error &e)
    {
        return -1;
        std::cerr << "In remove_remote(): " << e.what() << std::endl;
    }
    return 0;
}

std::string GitHandler::exec_cmd(const std::string cmd)
{
    if(!system_recognized()) return NULL;

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
    //strptime(tm_val.c_str(), "%c", &tm);
    t = mktime(&tm);
    return (int) t;
}
