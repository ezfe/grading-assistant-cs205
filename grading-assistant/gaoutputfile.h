#ifndef GAOUTPUTFILE_H
#define GAOUTPUTFILE_H

#include "gadata/gaassignmentdata.h"
#include "gadata/gaannotation.h"
#include <fstream>

/*!
 * @brief This class creates the html output file based on a student's assignment data.
 */
class GAOutputFile {
public:

    //Constructor/Destructor
    GAOutputFile(std::string fp, GAAssignmentData *d);
    ~GAOutputFile();

    //File methods
    void open_empty();

    void flush();

    void close();

    void write_to_file();

private:

    //Assignment Data
    GAAssignmentData *data;

    //File variables
    std::ofstream fileHandler;
    std::string filePath;

};

#endif // GAOUTPUTFILE_H

