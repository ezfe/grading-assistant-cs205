#include <githandler.h>


int main() {

    GitHandler g1;

    std::cout << g1.make_remote() << std::endl;
    g1.init_repo();
    //g1.load_repo();
    //g1.save_repo();
    //g1.remove_local();
    //g1.remove_remote();

    return 0;
}
