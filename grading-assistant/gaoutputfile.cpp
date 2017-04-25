#include "gaoutputfile.h"


//CONSTRUCTOR/DESTRUCTOR


/*!
 * @brief Takes in a file path and assignment data object and opens an empty file based on the
 * file path.
 * @param fp - file path
 * @param d - Assignment Data object
 */
GAOutputFile::GAOutputFile(std::string fp, GAAssignmentData *d) {
    data = d;
    filePath = fp;
    open_empty();
}


/*!
 * @brief Destructor; closes the file
 */
GAOutputFile::~GAOutputFile() {
    fileHandler.close();
}


//FILE METHODS


/*!
 * @brief Opens a new file based on the inputted file path and the student's information
 */
void GAOutputFile::open_empty() {

    try {
        std::string path = filePath + "/" + data->get_student()->get_lafayette_username() +
                "_" + data->get_assignment()->get_title() + ".html";

        fileHandler.open(path, std::ofstream::out | std::ofstream::trunc);
        write_to_file();

    } catch (std::exception &e) {
        std::cerr << "File opening error" << std::endl;
    }
}


/*!
 * @brief Actually writes the student's data to the file
 */
void GAOutputFile::write_to_file() {

    //Set up style preferences
    fileHandler << "<!DOCTYPE html><html><head></head><body>";
    fileHandler << "<style> table {font-family: arial, sans-serif; border-collapse: collapse; width: 100%; }";
    fileHandler << "td, th {border: 1px solid #dddddd; text-align: left; padding: 8px; }";
    fileHandler << "tr:nth-child(even) {background-color: #dddddd;} </style>";

    //print basic informatoin
    fileHandler << "<h1>Student: " + data->get_student()->get_name() + "</h1>";
    fileHandler << "<h2>" + data->get_student()->get_class()->get_name() + " - " +
                   data->get_student()->get_class()->get_semester() + " " +
                   data->get_student()->get_class()->get_year() + ": " +
                   data->get_assignment()->get_title() + " Grading Summary" + "</h2>";

    //print annotations
    GARubric* rubric = data->get_assignment()->get_rubric();
    for(GARubricRow* row: rubric->get_rows()) {
        fileHandler << "<h3>" + row->get_category() + "</h3>";
        std::vector<GAAnnotation*> annotations = data->get_by_category(row->get_category());
        fileHandler << "<ul style=\"list-style-type:none\">";
        for(GAAnnotation* annotation: annotations) {
            fileHandler << "<li>" + annotation->get_title() + ": " + annotation->get_description()
                           + "  " + annotation->get_location() + "</li>";
        }
        fileHandler << "</ul>";

    }

    //print EC annotations if applicable
    if(data->get_assignment()->get_rubric()->get_ec() != nullptr) {
        fileHandler << "<h3>Extra Credit</h3>";
        std::vector<GAAnnotation*> ec = data->get_by_category("Extra Credit");

        fileHandler << "<ul style=\"list-style-type:none\">";
        for(GAAnnotation* annotation: ec) {
            fileHandler << "<li>" + annotation->get_title() + ": " + annotation->get_description()
                           + "  " + annotation->get_location() + "</li>";
        }
        fileHandler << "</ul>";
    }

    //print out rubric table
    fileHandler << "<table style=\"width:100%\">";
    fileHandler << "<caption><h3>Grading Rubric</h3></caption>";

    //horizontal header
    fileHandler << "<tr><th>Category</th>";
    int cols = data->get_assignment()->get_rubric()->get_rows()[0]->get_descriptions().size();
    for(int l = 1; l <= cols; l++) {
        fileHandler << "<th>" + std::to_string(l) + "</th>";
    }
    fileHandler << "<th>Points</th></tr>";

    //go through each row
    for(GARubricRow* row: rubric->get_rows()) {
        fileHandler << "<tr><td>" + row->get_category() + "</td>";

        //print columns
        for(int n = 0; n < cols; n++) {
            fileHandler << "<td>" + row->get_descriptions()[n] + "</td>";
        }
        //print max points for row
        fileHandler << "<td>" + std::to_string(data->calculate_score(row)) +" / " +
                       std::to_string(row->get_max_points()) + "</td></tr>";
    }

    //add EC row if applicable
    if(data->get_assignment()->get_rubric()->get_ec() != nullptr) {
        fileHandler << "<tr><td>Extra Credit</td>";
        for(int p = 0; p < cols; p++) {
            if(p == 0) {
                fileHandler << "<td>" + data->get_assignment()->get_rubric()->get_ec()->
                               get_descriptions().front() + "</td>";
            }
            else
            {
                fileHandler << "<td></td>";
            }
        }
        fileHandler << "<td>" + std::to_string(data->calculate_score(data->get_assignment()->
                                                                     get_rubric()->get_ec())) +
                       " / " + std::to_string(data->get_assignment()->get_rubric()->get_ec()->
                                              get_max_points()) + "</td></tr>";
    }

    fileHandler << "<tr><td>Total Points</td>";
    for(int q = 0; q < cols; q++) {
        fileHandler << "<td></td>";
    }
    fileHandler << "<td>" + std::to_string(data->calculate_score()) + " / " +
                   std::to_string(data->get_assignment()->
                                  get_rubric()->get_max_points()) + "</td></tr>";

    fileHandler << "</table>";

    //print out final score
    fileHandler << "<h2>Final Score: " + std::to_string(data->calculate_percentage()) + "%</h2>";

    fileHandler << "</body></html>";
    close();
}


/*!
 * @brief Flushes the file
 */
void GAOutputFile::flush() {
    try {

        fileHandler.flush();

    } catch (std::exception &e) {

        std::cerr << "Error with flushing file" << std::endl;
    }
}


/*!
 * @brief Closes the file
 */
void GAOutputFile::close() {
    try {

        fileHandler.close();

    } catch(std::exception &e) {

        std::cerr << "File closing error" << std::endl;
    }
}
