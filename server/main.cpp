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
//#include <iomanip>
//#include <time.h>

char buf[4096]; // never know how much is needed

int main() {

    std::string currDir = FileManager::get_app_directory();

    chdir(FileManager::get_app_directory().c_str());

    //git init
    //git add .
    //git commit -m 'initial commit'
    //git remote add origin ssh://spr2017_l2g4@139.147.9.185:/home/spr2017_l2g4/repo_server.git
    //git push origin master
    //system("git init --bare --shared repo_server.git");
    //system("exit");

    std::cout << currDir << std::endl;
    std::cout << getcwd(buf, sizeof(buf)) << std::endl;


    /*
    std::cout  << "CWD: " << cwd(buf, sizeof buf) << std::endl;

    // Change working directory and test for success
    cd("../../");

    std::cout << "CWD changed to: " << cwd(buf, sizeof buf) << std::endl;

    cd("repo_test");

    system("touch stuff.txt");

    system("echo \"some stuff \" >> stuff.txt");

    std::cout << "CWD changed to: " << cwd(buf, sizeof buf) << std::endl;

    system("git status");

    system("git add .");

    system("git commit -m \"pushing stuff.txt\"");

    system("git push");
    */
    /*
    struct tm tm1;
    struct tm tm2;
    struct tm tm3;
    time_t t1;
    time_t t2;
    time_t t3;
    strptime("Tue Apr 11 20:13:07 2017", "%c", &tm1);

    strptime("Tue Apr 11 20:06:13 2017", "%c", &tm2);

    strptime("Tue Apr 11 19:15:48 2017", "%c", &tm3);


    t1 = mktime(&tm1);
    t2 = mktime(&tm2);
    t3 = mktime(&tm3);

    std::cout << t1 << "\n" << t2 << "\n" << t3 << "\n";
    */

    return 0;
}
