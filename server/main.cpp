#include <githandler.h>


int main() {

   //GitHandler g1;
    std::cout << FileManager::get_app_directory() << std::endl;
    FileManager::assure_directory_exists(FileManager::get_app_directory());
    _chdir(FileManager::get_app_directory().c_str());
    system("echo \"Hello World!\" >> helloworld.txt");
    //std::cout << g1.make_remote() << std::endl;
    //system("git init");
    //g1.init_repo();
    //g1.load_repo();
    //g1.save_repo();
    //g1.remove_local();
    //g1.remove_remote();

    return 0;
}
