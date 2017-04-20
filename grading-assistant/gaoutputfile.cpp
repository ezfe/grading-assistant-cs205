#include "gaoutputfile.h"

GAOutputFile::GAOutputFile(std::string fp, GAAssignmentData *d) {
    data = d;
    filePath = fp;
    open_empty();
}

GAOutputFile::~GAOutputFile() {
    fileHandler.close();
}

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

void GAOutputFile::write_to_file() {

    fileHandler << "<!DOCTYPE html><html><head></head><body>";
    fileHandler << "<style> table {font-family: arial, sans-serif; border-collapse: collapse; width: 100%; }";
    fileHandler << "td, th {border: 1px solid #dddddd; text-align: left; padding: 8px; }";
    fileHandler << "tr:nth-child(even) {background-color: #dddddd;} </style>";
    fileHandler << "<h1>Student: " + data->get_student()->get_name() + "</h1>";
    fileHandler << "<h2>" + data->get_assignment()->get_title() + " Grading Summary" + "</h2>";

    GARubric* rubric = data->get_assignment()->get_rubric();
    for(GARubricRow* row: rubric->get_rows()) {
        fileHandler << "<h3>" + row->get_category() + "</h3>";
        std::vector<GAAnnotation*> annotations = data->get_by_category(row->get_category());
        fileHandler << "<ul style=\"list-style-type:none\">";
        for(GAAnnotation* annotation: annotations) {
            fileHandler << "<li>" + annotation->get_title() + ": " + annotation->get_description() + "     " + annotation->get_location() + "</li>";
        }
        fileHandler << "</ul>";

    }

    if(data->get_assignment()->get_rubric()->get_ec() != nullptr) {
        fileHandler << "<h3>Extra Credit</h3>";
        std::vector<GAAnnotation*> ec = data->get_by_category("Extra Credit");

        fileHandler << "<ul style=\"list-style-type:none\">";
        for(GAAnnotation* annotation: ec) {
            fileHandler << "<li>" + annotation->get_title() + ": " + annotation->get_description() + "     " + annotation->get_location() + "</li>";
        }
        fileHandler << "</ul>";
    }


    fileHandler << "<table style=\"width:100%\">";
    fileHandler << "<caption><h3>Grading Rubric</h3></caption>";

    fileHandler << "<tr><th>Category</th>";
    int cols = data->get_assignment()->get_rubric()->get_rows()[0]->get_descriptions().size();
    for(int l = 1; l <= cols; l++) {
        fileHandler << "<th>" + std::to_string(l) + "</th>";
    }
    fileHandler << "<th>Points</th></tr>";

    for(GARubricRow* row: rubric->get_rows()) {
        fileHandler << "<tr><td>" + row->get_category() + "</td>";
        for(int n = 0; n < cols; n++) {
            fileHandler << "<td>" + row->get_descriptions()[n] + "</td>";
        }
        fileHandler << "<td>" + std::to_string(data->calculate_score(row)) +" / " + std::to_string(row->get_max_points()) + "</td></tr>";
    }

    if(data->get_assignment()->get_rubric()->get_ec() != nullptr) {
        fileHandler << "<tr><td>Extra Credit</td>";
        for(int p = 0; p < cols; p++) {
            if(p == 0) {
                fileHandler << "<td>" + data->get_assignment()->get_rubric()->get_ec()->get_descriptions().front() + "</td>";
            }
            else
            {
                fileHandler << "<td></td>";
            }
        }
        fileHandler << "<td>" + std::to_string(data->calculate_score(data->get_assignment()->get_rubric()->get_ec())) +
                       " / " + std::to_string(data->get_assignment()->get_rubric()->get_ec()->get_max_points()) + "</td></tr>";
    }

    fileHandler << "<tr><td>Total Points</td>";
    for(int q = 0; q < cols; q++) {
        fileHandler << "<td></td>";
    }
    fileHandler << "<td>" + std::to_string(data->calculate_score()) + " / " + std::to_string(data->get_assignment()->
                                                                              get_rubric()->get_max_points()) + "</td></tr>";

    fileHandler << "</table>";

    fileHandler << "</body></html>";
    close();
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
