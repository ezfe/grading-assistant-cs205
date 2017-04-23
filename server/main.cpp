#include <githandler.h>


int main() {

    GitHandler g1;
    std::cout << g1.make_remote() << std::endl;
    g1.init_repo();
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
