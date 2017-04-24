#include <githandler.h>


int main() {

    GitHandler g1("spr2017_l2g4", "139.147.9.185", "/home/spr2017_l2g4/", "repo_server.git");
    g1.setup();
    std::cout << "Error code: " << g1.get_errors() << std::endl;
    g1.clear_errors();

    g1.sync();
    std::cout << "Error code: " << g1.get_errors() << std::endl;
    g1.clear_errors();

    //g1.resolve();
    //std::cout << "Error code: " << g1.get_errors() << std::endl;
    //g1.clear_errors();

    //g1.remove_remote();

    //g1.load_repo();
    //g1.save_repo();
    //g1.remove_local();
    //g1.remove_remote();
/*
    chdir(FileManager::get_app_directory().c_str());
    std::string test;
    test += "@";
    test += g1.exec_cmd("git push");
    test += "@";
    std::cout << test << std::endl;

    test = "";
    test += "@";
    test += g1.exec_cmd("git status");
    test += "@";
    std::cout << test << std::endl;
*/
    return 0;
}
