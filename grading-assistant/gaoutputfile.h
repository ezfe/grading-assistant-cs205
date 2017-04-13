#ifndef GAOUTPUTFILE_H
#define GAOUTPUTFILE_H

#include "gadata/gaassignmentdata.h"
#include "gadata/gaannotation.h"
#include <fstream>

class GAOutputFile {
public:
    //GAOutputFile();
    GAOutputFile(std::string fp, GAAssignmentData *d);
    ~GAOutputFile();

    void open_empty();
    void flush();
    void close();
    void write_to_file();

private:
    GAAssignmentData *data;
    std::ofstream fileHandler;
    std::string filePath;

};

#endif // GAOUTPUTFILE_H

