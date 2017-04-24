#include <githandler.h>


int main() {

    GitHandler g1("spr2017_l2g4", "139.147.9.185", "/home/spr2017_l2g4/repo_server.git");
    g1.setup();
    std::cout << "Error code: " << g1.get_errors() << std::endl;
    g1.clear_errors();

    g1.sync();
    std::cout << "Error code: " << g1.get_errors() << std::endl;
    g1.clear_errors();

    //g1.resolve();
    std::cout << "Error code: " << g1.get_errors() << std::endl;
    g1.clear_errors();

    //g1.remove_remote();

    return 0;
}
