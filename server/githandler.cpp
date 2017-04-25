#include "githandler.h"

/*!
 * \brief GitHandler::GitHandler
 *
 * Constructor for GitHandler object.
 *
 * Once created, be sure to check the system is recognized (Apple/Linux/Windows).
 *
 * Assumes user has set up SSH keys for remote location.
 *
 * \param user User of SSH location for Git Repository e.g. spr2017_l2g4
 * \param host Host of SSH location for Git Repository e.g. 139.147.9.185
 * \param remotePath Path of SSH location for Git Repository e.g. /home/spr2017_l2g4/repo_server.git
 */
GitHandler::GitHandler(std::string user, std::string host, std::string remotePath) {
    this->recsys = system_recognized();

    // Ensure that our (possibly future) repo exists
    FileManager::assure_directory_exists(FileManager::get_app_directory());

    // Default values for repo location
    this->remoteURL  = user + "@" + host;
    this->remotePath = remotePath;

    this->localPath = "\"" + FileManager::get_app_directory() + "\"";

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
 * Currently implemented: Apple, Linux, Windows
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
void GitHandler::set_remote_url(const std::string loc)
{
    this->remoteURL = loc;
}


/*!
 * \brief GitHandler::get_remote_loc
 *
 * This method returns the remote location of the Git Repository
 *
 * \return std::string The remote location of the Git Repository
 */
std::string GitHandler::get_remote_url()
{
    return this->remoteURL;
}

/*!
 * \brief GitHandler::set_remote_path
 *
 * This method sets the remote path of the Git Repository
 *
 * Must be in the format: /path/to/location/repo_name.git
 * e.g. /home/user05/repo_stuff.git
 *
 * \param path The user-defined path to Git Repository will reside
 */
void GitHandler::set_remote_path(const std::string path)
{
    this->remotePath = path;
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
    return this->remotePath;
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
    this->localPath = loc;
}

/*!
 * \brief GitHandler::get_repo_loc
 *
 *
 * \return path The path of the local Git Repository
 */
std::string GitHandler::get_repo_loc(void)
{
    return this->localPath;
}

/*!
 * \brief GitHandler::get_errors
 *
 * Method for checking error flags while initializing/loading/saving repository.
 *
 * \return
 *
 * 0: No errors detected.
 * 1: Issue while initializing remote.
 * 2: Issue while pulling.
 * 3: Issue while pushing.
 * 4: Issue with both pushing and pulling.
 *
 */
int GitHandler::get_errors()
{
//    if(remotefail) return 1;
//    else if(pullfail & pushfail) return 4;
//    else if(pullfail) return 2;
//    else if(pushfail) return 3;
//    else return 0;
    return 0;
}

/*!
 * \brief GitHandler::clear_errors
 *
 * Clears all error flags
 *
 */
void GitHandler::clear_errors()
{
    this->remotefail = false;
    this->pullfail   = false;
    this->pushfail   = false;
}

/*!
 * \brief GitHandler::setup
 *
 * Calls private methods that perform remote/local initialization/reinitialization
 *
 * Be sure to follow this call with a get_errors() to ensure no issues were encountered
 *
 */
void GitHandler::setup() {
    make_remote();
    remove_local();
    init_repo();
}

/*!
 * \brief GitHandler::sync
 *
 * Calls private methods that perform loading/saving of Git Repository
 *
 * Be sure to follow this call with a get_errors() to ensure no issues were encountered
 *
 */
void GitHandler::sync(void) {
    load_repo();
    save_repo();
}

/*!
 * \brief GitHandler::resolve
 *
 * Removes local repository, then initializes and pulls
 *
 * \return int The error value generated when setting up and syncing the replaced Git Repository.
 *
 */
int GitHandler::resolve(void) {
    remove_local();

    FileManager::assure_directory_exists(FileManager::get_app_directory());

    setup();

    sync();

    return get_errors();
}

/*!
 * \brief GitHandler::remove_local
 *
 * Removes local repository (deletion)
 *
 */
void GitHandler::remove_local(void)
{
    QDir appDir(QString::fromStdString(FileManager::get_app_directory()));
    appDir.removeRecursively();
}

/*!
 * \brief GitHandler::make_remote
 *
 * Private method that initializes a bare Git Repository at the location specified
 * when the GitHandler object is constructed.
 *
 * \return int
 * -1: Exception caught
 * 0: Successful attempt at remote instantiation
 */
int GitHandler::make_remote(void)
{
    std::string command, rtn;
    size_t init, reinit;

    command += "ssh " + remoteURL;
    command += " \"git init --bare " + remotePath + "\"";

    try{
        change_dir(FileManager::get_app_directory());

        rtn    = exec_cmd(command);
        init   = rtn.find("Initialized");
        reinit = rtn.find("Reinitialized");

        if((init == std::string::npos) && (reinit == std::string::npos))
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

/*!
 * \brief GitHandler::init_repo
 *
 * Private method that initializes a Git Repository in the application directory.
 *
 * Adds remote as specified in object construction.
 *
 * \return int
 * -1: System unrecognized, or exception caught
 * 0: Successful attempt at local Git Repository instantiation
 */
int GitHandler::init_repo(void)
{
    if(!system_recognized()) return -1;

    try
    {
        FileManager::assure_directory_exists(FileManager::get_app_directory());
        change_dir(FileManager::get_app_directory());

        std::string command, rtn;
        // Add the remote
        exec_cmd("git init");
        command = "git remote add origin ssh://" + remoteURL + ":" + remotePath;
        exec_cmd(command);

        this->load_repo();

        std::ofstream fh;
        fh.open(FileManager::append(FileManager::get_app_directory(), "INITLOG.txt"), std::ofstream::out | std::ofstream::trunc);
        fh << std::to_string(get_time_stamp()) << std::endl;
        fh.flush();
        fh.close();

        exec_cmd("git add .; git commit -am \"Configuration Commit\";");
        this->save_repo();

        // Pull the remote changes
        this->load_repo();

        if (remotefail) {
            std::cerr << "Git remote issue potentially detected. "
                         "Confirm correct remote path.\n"
                      << rtn << std::endl;
            return -1;
        }
    }
    catch(std::runtime_error &e)
    {
        return -1;
        std::cerr << "In init_repo(): " << e.what() << std::endl;
    }
    return 0;
}

/*!
 * \brief GitHandler::load_repo
 *
 * Private method that pulls from remote Git Repository
 *
 * \return int
 * -1: System unrecognized, or exception caught
 * 0: Successful attempt at pulling from the Git Repository
 */
int GitHandler::load_repo(void)
{
    std::string command, rtn;
    size_t err;

    if(!system_recognized()) return -1;

    try
    {
        change_dir(FileManager::get_app_directory());

        command = "git pull origin master --quiet";
        exec_cmd(command);

        // Pull again so string may be returned and buffered (for check)
        command = "git pull origin master";
        rtn = exec_cmd(command);
        err   = rtn.find("Already up");
        if(err == std::string::npos)
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

/*!
 * \brief GitHandler::save_repo
 *
 * Private method that adds, commits, and pushes to remote Git Repository
 *
 * \return int
 * -1: System unrecognized, or exception caught
 * 0: Successful attempt at pushing to the Git Repository
 */
int GitHandler::save_repo(void)
{
    std::string command, rtn;
    size_t ntc;

    if(!system_recognized()) return -1;

    try {
        change_dir(FileManager::get_app_directory());

        rtn = exec_cmd("git status");
        ntc = rtn.find("nothing to commit");

        if(ntc == std::string::npos) {
            command = "git add .;git commit -am \"";
            command.append(std::to_string(get_time_stamp()));
            command += "\"";
            exec_cmd(command);
        }

        command = "git push origin master";
        exec_cmd(command);
    } catch(std::runtime_error &e) {
        return -1;
        std::cerr << "In save_repo(): " << e.what() << std::endl;
    }
    return 0;
}

/*!
 * \brief GitHandler::change_dir
 *
 * Private method used to change the current working directory.
 *
 * Recognizes and attempts directory change on Apple/Linux/Windows systems.
 *
 * \param path The directory to change to
 */
void GitHandler::change_dir(const std::string path)
{
    if (this->recsys) {
        cd(path.c_str());
    } else {
        std::cerr << "System not recognized, directory change command not known" << std::endl;
    }
}

/*!
 * \brief GitHandler::exec_cmd
 *
 * Private method used to execute a system() command and return output in a std::string
 *
 * \param cmd The command to have system() execute
 *
 * \return std::string The returned text from the system() call
 */
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

/*!
 * \brief GitHandler::get_time_stamp
 *
 * Private method used to return a time stamp. Used as commit values when syncing with remote.
 *
 * \return int Time and Date
 */
int GitHandler::get_time_stamp(void)
{
    time_t rawtime;
    time(&rawtime);
    return (int)time(&rawtime);
}
