//#ifdef _WIN32
//#include <direct.h>
//#define cwd _getcwd
//#define cd _chdir
//#else
#include "unistd.h"
#define cwd getcwd
#define cd chdir
//#endif

#include <iostream>
#include <sstream>
#include <../grading-assistant/filemanager.h>
#include <githandler.h>

//#include <time.h>

char buf[4096]; // never know how much is needed

int main() {

    GitHandler g1;

    std::string command = "ssh spr2017_l2g4@139.147.9.185 ls -d *repo_server.git*";

    std::cout << g1.make_remote() << std::endl;
    g1.init_repo();
    g1.clone_repo();

    std::string before = "[ master 245234 ]";
    std::cout << g1.partial_string(before, 2) << std::endl;


    //std::string after = before.find_first_of(" \t");
    //after = after.substr(after.find_first_of(" \t") + 1);


    //g1.make_remote();

    //chdir(FileManager::get_app_directory().c_str());

    //git init
    //git add .
    //git commit -m 'initial commit'
    //git remote add origin ssh://spr2017_l2g4@139.147.9.185:/home/spr2017_l2g4/repo_server.git
    //git push origin master
    //system("git init --bare --shared repo_server.git");
    //system("exit");

    //std::cout << currDir << std::endl;
    //std::cout << getcwd(buf, sizeof(buf)) << std::endl;




    return 0;
}
