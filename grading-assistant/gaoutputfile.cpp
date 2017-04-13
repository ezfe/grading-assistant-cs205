#include "gaoutputfile.h"

GAOutputFile::GAOutputFile(GAAssignmentData *d) {
    data = d;

    open_empty();
}

GAOutputFile::~GAOutputFile() {
    fileHandler.close();
}

void GAOutputFile::open_empty() {
    try {

        std::string path = "/home/sampsell/Desktop/" + data->get_student()->get_lafayette_username() +
                "_" + data->get_assignment()->get_title() + ".html";

        fileHandler.open(path, std::ofstream::out | std::ofstream::trunc);

    } catch (std::exception &e) {
        std::cerr << "File opening error" << std::endl;
    }
}

void GAOutputFile::write_to_file() {

}

void GAOutputFile::flush() {
    try {

        fileHandler.flush();

    } catch (std::exception &e) {

        std::cerr << "Error with flushing file" << std::endl;
    }
}

void GAOutputFile::close() {
    try {

        fileHandler.close();
    } catch(std::exception &e) {

        std::cerr << "File closing error" << std::endl;
    }
}
