#include "filemanager.h"

FileManager::FileManager() {

}

/*!
 * \brief Expand tilde character to home directory
 *
 * If the first two characters of the string are `~/` then
 * the tilde will be replaced with the contents of $HOME
 *
 * Example: `~/test` ==> `/Users/ezekielelin/test`
 *
 * \param path The path to process
 * \return  The expanded path
 */
std::string FileManager::expand_home(std::string path) {
    QString qpath = QString::fromStdString(path);
    QString ret_val;
    if (qpath.at(0) == '~' && qpath.at(1) == '/') {
        char* temp = getenv("HOME");
        if (temp != NULL) {
            QString userPath = QString(temp);
            return QDir::cleanPath(userPath + "/" + qpath.mid(1)).toStdString();
        } else {
            std::cerr << "Unable to get HOME" << std::endl;
            return QDir::cleanPath(qpath).toStdString();
        }
    } else {
        return QDir::cleanPath(qpath).toStdString();
    }
}

/*!
 * \brief Recursively create a directory if it doesn't exist
 *
 * If the directory exists, nothing will happen. If the directory doesn't exist, the _entire_
 * path will be created.
 *
 * \param path The path to create
 */
void FileManager::assure_directory_exists(std::string path) {
    QDir dir(QString::fromStdString(path));
    if (!dir.exists()) {
        dir.mkpath(".");
    }
}

/*!
 * \brief Get the data folder path
 *
 * This folder is where all persistent data the user stores should go.
 *
 * - macOS: ~[home]/Library/Application Support/APPNAME/
 * - linux: ~[home]/.APPNAME/
 * - Windows: ???
 *
 * \return The data folder path
 */
std::string FileManager::get_app_directory() {
    const std::string app_name = "elin-sampsell.grading-assistant";
    std::string ret_val;
    if (GA_PLATFORM == GA_PLATFORM_APPLE) {
        ret_val = FileManager::expand_home("~/Library/Application Support/" + app_name + "/");
    } else if (GA_PLATFORM == GA_PLATFORM_LINUX) {
        ret_val = FileManager::expand_home("~/." + app_name + "/");
    } else {
        ret_val = "./" + app_name + "/";
    }
    return QDir::cleanPath(QString::fromStdString(ret_val)).toStdString();
}

/*!
 * \brief Get the settings file path
 *
 * This file is managed by UserSettings
 *
 * \return The settings file path
 */
std::string FileManager::get_settings_path() {
    return QDir::cleanPath(QString::fromStdString(FileManager::get_app_directory() + "/settings.txt")).toStdString();
}

/*!
 * \brief Get the database file path
 * \return The database file path
 */
std::string FileManager::get_database_path() {
    return QDir::cleanPath(QString::fromStdString(FileManager::get_app_directory() + "/database.sqlite3")).toStdString();
}

/*!
 * \brief Get a class's folder path
 * \param class_ The class
 * \return The class's folder path
 */
std::string FileManager::get_class_directory(GAClass* class_) {
    std::string directory = FileManager::get_app_directory();
    std::string classID = class_->get_id();
    std::string classPath = directory + "/assignment-data/class-" + classID + "/";
    return QDir::cleanPath(QString::fromStdString(classPath)).toStdString();
}

/*!
 * \brief Get an assignment's folder path
 *
 * This will be inside the correct class folder
 *
 * \param assignment The assignment
 * \return The assignment's folder path
 */
std::string FileManager::get_assignment_directory(GAAssignment* assignment) {
    std::string classDirectory = FileManager::get_class_directory(assignment->get_class());
    std::string assignmentID = assignment->get_id();
    std::string assignmentPath = classDirectory + "/assigment-" + assignmentID + "/";
    return QDir::cleanPath(QString::fromStdString(assignmentPath)).toStdString();
}

/*!
 * \brief Get an student's assignment folder path
 *
 * This is a folder for each student inside the assignment folder
 *
 * \param assignment The assignment
 * \param student The student
 * \return The student's assignment path
 */
std::string FileManager::get_assignment_student_directory(GAAssignment* assignment, GAStudent* student) {
    std::string assignmentDirectory = FileManager::get_assignment_directory(assignment);
    std::string studentID = student->get_id();
    std::string fullpath = FileManager::append(assignmentDirectory, "/student-data-" + studentID + "/");
    return fullpath;
}

/*!
 * \brief Get a student's assignment folder path
 *
 * This is a folder for each student inside the assignment folder. This version uses the GAAssignmentData object
 * instead of the student and assignment objects
 *
 * \param assignment_data The assignment data
 * \return The studen'ts assignment path
 */
std::string FileManager::get_assignment_student_directory(GAAssignmentData* assignment_data) {
    return FileManager::get_assignment_student_directory(assignment_data->get_assignment(), assignment_data->get_student());
}

/*!
 * \brief Combine two paths
 *
 * Example: `~/Home` & 'Library' => `~/Home/Library`
 * Example: `~/Home/` & '/Library/' => `~/Home/Library`
 *
 * Duplicate /'s will automatically be cleaned up
 *
 * \param path The first path
 * \param appending The path to append
 * \return The final path
 */
std::string FileManager::append(std::string path, std::string appending) {
    return QDir::cleanPath(QString::fromStdString(path + "/" + appending)).toStdString();
}

/*!
 * \brief Combine three paths
 *
 * See append(std::string path, std::string appending) for details, this just calls that.
 *
 * \param path The first path
 * \param appending The first path to append
 * \param appending_2 The second path to append
 * \return The final path
 */
std::string FileManager::append(std::string path, std::string appending, std::string appending_2) {
    return FileManager::append(FileManager::append(path, appending), appending_2);
}

/*!
 * \brief Copy a directory into the grading-assistant data folder
 *
 * dir/[laf username]/files...
 *
 * \param path The path to the directory
 * \param ga The grading assistant
 * \return The
 */
void FileManager::import(std::string path, GradingAssistant* ga, GAAssignment* assign) {
    QDir importDir(QString::fromStdString(path));
    importDir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot | QDir::NoSymLinks);
    QDirIterator it(importDir, QDirIterator::NoIteratorFlags);
    while (it.hasNext()) {
        QString path = it.next();
        QDir studentDir(path);
        GAStudent* student = ga->get_student(studentDir.dirName().toStdString());
        if (student == nullptr) {
            std::cout << "Create student!" << studentDir.dirName().toStdString() << std::endl;
            continue;
        }
        std::string intendedPath = FileManager::get_assignment_student_directory(assign, student);
        QDir intendedDir(QString::fromStdString(intendedPath));
        intendedDir.removeRecursively();
        FileManager::assure_directory_exists(FileManager::get_assignment_directory(assign));
        QDir dir;
        dir.rename(studentDir.absolutePath(), intendedDir.absolutePath());
    }
}

/*!
 * \brief Get a list of files in the directory
 * \param path The directory
 */
std::vector<std::pair<std::string, std::string>> FileManager::get_files_in(std::string path) {
    std::vector<std::pair<std::string, std::string>> ret;
    QDir root(QString::fromStdString(path));
    root.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot | QDir::NoSymLinks);
    QDirIterator it(root, QDirIterator::Subdirectories);
    while(it.hasNext()) {
        QString path = it.next();
        QFileInfo fi(path);
        std::pair<std::string, std::string> item;
        item.first = fi.fileName().toStdString();
        item.second = path.toStdString();
        ret.push_back(item);
    }
    return ret;
}
