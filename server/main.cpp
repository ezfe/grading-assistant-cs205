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
#include <../grading-assistant/filemanager.h>
#include <githandler.h>

//#include <time.h>

char buf[4096]; // never know how much is needed

int main() {

    GitHandler g1;

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
