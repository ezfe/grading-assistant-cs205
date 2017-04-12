#ifndef GITHANDLER_H
#define GITHANDLER_H

#include <iostream>


class GitHandler
{
public:
    GitHandler();
    ~GitHandler();

    void set_repo_loc(std::string path); // - Could use UserSettings?
    std::string get_repo_loc();




private:

    /*!
     * \brief store_commit_val
     * Stores commit value after push. Later used to determine if
     * branch is furthest ahead.
     *
     * \param val
     */
    void store_commit_val(std::string val);

    std::string repoloc;
    std::string currcommit;



};

#endif // GITHANDLER_H
