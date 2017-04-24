#include "githandler.h"

// *** Have remote path include /home

// Constructor take:user, host, path(include repo name)
// setup(): Make sure all initialization has occurred (remote + local)
// get_errors(): integer 0 = OK - Get flags set by setup concerning issues encountered 1 - PULL FAIL, 2 - CONFLICTS

// Only sync() after init() - that way issues only arise in one location

/*!
 * \brief GitHandler::GitHandler
 *
 * Constructor for GitHandler object.
 *
 * Once created, be sure to check the system is recognized (Apple/Linux/Windows).
 *
 * Sets default values for remote location, remote path, and repo name as follows:
 *
 * spr2017_l2g4@139.147.9.185
 * home/spr2017_l2g4/
 * repo_server.git
 *
 * Assumes user has set up SSH keys for remote location.
 *
 * Notes: On Windows machines, remotes must be created/destroyed manually.
 *        This may be done through PuTTY or Git Bash.
 *        Further instructions may be found in the instructions.txt file included in project.
 *
 */
GitHandler::GitHandler(std::string user, std::string host, std::string path, std::string name)
{
    recsys = system_recognized();

    // Ensure that our (possibly future) repo exists
    FileManager::assure_directory_exists(FileManager::get_app_directory());

    // Default values for repo location
    remoteloc  = user + "@" + host;
    remotepath = path;

    repoloc    = "\"" + FileManager::get_app_directory() + "\"";
    reponame   = name;

    clear_errors();
}

/*!
 * \brief GitHandler::~GitHandler
 *
 * Destructor for GitHandler Class
 *
 */
GitHandler::~GitHandler(){}

/*!
 * \brief GitHandler::system_recognized
 *
 * This method identifies the system, and returns a boolean value based
 * upon whether or not GitHandler operations may be done on the system.
 *
 * Currently implemented: Apple, Linux, Windows (note not all functions for Windows)
 *
 * \return bool If the system is recognized (and supported)
 */
bool GitHandler::system_recognized(void)
{
    if(GA_PLATFORM == GA_PLATFORM_APPLE)        return true;
    else if(GA_PLATFORM == GA_PLATFORM_LINUX)   return true;
    else if(GA_PLATFORM == GA_PLATFORM_WINDOWS) return true;
    else return false;
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

/*!
 * \brief GitHandler::set_repo_loc
 *
 * This method sets the Git Repository location
 *
 * It is recommended that static method FileManager::get_app_directory() be used
 *
 * \param loc The path to the Git Repository (will be app directory)
 */
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

int GitHandler::get_errors()
{
    if(remotefail) return 1;
    else if(pullfail & pushfail) return 4;
    else if(pullfail) return 2;
    else if(pushfail) return 3;
    else return 0;
}

void GitHandler::clear_errors()
{
    this->remotefail = false;
    this->pullfail   = false;
    this->pushfail   = false;
}

int GitHandler::make_remote(void)
{
    std::string command, rtn;
    size_t init, reinit;

    command += "ssh " + remoteloc;
    command += " \"git init --bare --shared ";
    command += reponame + "\"";

    try{
        change_dir(FileManager::get_app_directory());

        rtn    = exec_cmd(command);
        init   = rtn.find("Initialized");
        reinit = rtn.find("Reinitialized");

        if((init == std::string::npos) && reinit == std::string::npos)
        {
            this->remotefail = true;
            return -1;
        }
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
        change_dir(FileManager::get_app_directory());

        std::string command;

        // Initialize the repo
        command = "git init --quiet";
        exec_cmd(command);

        // Add the remote - first check if exists
        std::string testremote;
        testremote.append(exec_cmd("git remote -v"));

        // If doesn't exist - add it
        if(!testremote.compare(""))
        {
            command = "git remote add origin ssh://";
            command += remoteloc + ":";
            command += remotepath;
            command += reponame;
            exec_cmd(command);
        }
    }
    catch(std::runtime_error &e)
    {
        return -1;
        std::cerr << "In init_repo(): " << e.what() << std::endl;
    }
    return 0;
}

void GitHandler::setup()
{
    make_remote();
    init_repo();
}

int GitHandler::load_repo(void)
{
    std::string command, rtn;
    size_t autd;

    if(!system_recognized())
    {
        this->pullfail = true;
        return -1;
    }

    try
    {
        change_dir(FileManager::get_app_directory());

        // Pull once (quietly)
        command = "git pull origin master --quiet";
        exec_cmd(command);

        // Pull again so string may be returned and buffered (for check)
        command = "git pull origin master";
        rtn   = exec_cmd(command);
        autd   = rtn.find("Already up");

        if(autd == std::string::npos)
        {
            this->pullfail = true;
            return -1;
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
    std::string command, rtn;
    size_t ntc;

    if(!system_recognized())
    {
        this->pushfail = true;
        return -1;
    }

    try
    {
        change_dir(FileManager::get_app_directory());

        rtn = exec_cmd("git status");
        ntc = rtn.find("nothing to commit");

        if(ntc == std::string::npos)
        {
            exec_cmd("git add . --quiet");

            command = "git commit -m \"";
            command.append(std::to_string((get_time_stamp())));
            command += "\" --quiet";
            exec_cmd(command);

            command = "git push -set-upstream origin master";
            exec_cmd(command);
        }
    }
    catch(std::runtime_error &e)
    {
        return -1;
        std::cerr << "In save_repo(): " << e.what() << std::endl;
    }
    return 0;
}

void GitHandler::sync(void)
{
    load_repo();
    save_repo();
}

int GitHandler::resolve(void)
{
    remove_local();

    FileManager::assure_directory_exists(FileManager::get_app_directory());

    setup();

    sync();

    return get_errors();
}

int GitHandler::remove_local(void)
{
    if(!system_recognized()) return -1;

    try
    {
        std::string command;

        if((GA_PLATFORM == GA_PLATFORM_APPLE) ||
           (GA_PLATFORM == GA_PLATFORM_LINUX))
        {
            command = "rm -rf ";
            command.append(FileManager::get_app_directory());
        }
        else if(GA_PLATFORM == GA_PLATFORM_WINDOWS)
        {
            command = "rd /s /q ";
            command += "\"";
            command.append(FileManager::get_app_directory());
            command += "\"";
        }
        exec_cmd(command);
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
    try
    {
        std::string command;

        command = "ssh ";
        command += remoteloc;

        command += " \"rm -rf ";
        command += reponame;
        command += "\"";

        exec_cmd(command);

        if(GA_WINDOWS_ACTIVE)
        {
            std::cerr << "Program attempted remote removal. "
                         "Confirm success (SSH protocol)" << std::endl;
        }
    }
    catch(std::runtime_error &e)
    {
        return -1;
        std::cerr << "In remove_remote(): " << e.what() << std::endl;
    }
    return 0;
}

void GitHandler::change_dir(const std::string path)
{
    if((GA_PLATFORM == GA_PLATFORM_APPLE) || (GA_PLATFORM == GA_PLATFORM_LINUX))
    {
        cd(path.c_str());
    }
    else if(GA_PLATFORM == GA_PLATFORM_WINDOWS)
    {
        cd(path.c_str());
    }
    else std::cout << "System not recognized, directory change command not known" << std::endl;
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

std::string GitHandler::partial_string(const std::__cxx11::string orig, const int numwords)
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
