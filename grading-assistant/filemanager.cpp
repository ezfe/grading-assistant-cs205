#include "filemanager.h"

FileManager::FileManager() {

}

/*!
 * \brief The value of $HOME or QDir::homePath()
 * \return The path
 */
std::string FileManager::get_home() {
    return QDir::homePath().toStdString();
}

/*!
 * \brief Expand tilde character to home directory
 *
 * If the first two characters of the string are `~/` then
 * the tilde will be replaced with the contents of FileManager::get_home()
 *
 * Example: `~/test` ==> `/Users/ezekielelin/test`
 *
 * \param path The path to process
 * \return  The expanded path
 */
std::string FileManager::expand_home(std::string path) {
    QString qpath = QString::fromStdString(path);
    if (qpath.at(0) == '~' && qpath.at(1) == '/') {
        QString userPath = QString::fromStdString(FileManager::get_home());
        return FileManager::append(userPath.toStdString(), qpath.mid(1).toStdString());
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
 * \brief Get the directory where the app directory will go
 * \return The directory path
 */
std::string FileManager::get_app_directory_location() {
    std::string ret_val;
    if (GA_PLATFORM == GA_PLATFORM_APPLE) {
        ret_val = FileManager::expand_home("~/Library/Application Support/");
    } else {
        ret_val = FileManager::expand_home("~/");
    }
    return QDir::cleanPath(QString::fromStdString(ret_val)).toStdString();

}

/*!
 * \brief Get the app name
 * \return The app name
 */
std::string FileManager::get_app_name() {
    return "elin-sampsell.grading-assistant";
}

/*!
 * \brief Get the data folder path
 *
 * This folder is where all persistent data the user stores should go.
 *
 * - macOS: $HOME/Library/Application Support/APPNAME/
 * - linux: $HOME/.APPNAME/
 * - Windows: C:/Users/Username/.APPNAME/
 *
 * \return The data folder path
 */
std::string FileManager::get_app_directory() {
    if (GA_PLATFORM == GA_PLATFORM_APPLE) {
        return FileManager::append(FileManager::get_app_directory_location(), FileManager::get_app_name());
    } else {
        return FileManager::append(FileManager::get_app_directory_location(), "." + FileManager::get_app_name());
    }
}

/*!
 * \brief Get the settings file path
 *
 * This file is managed by UserSettings
 *
 * \return The settings file path
 */
std::string FileManager::get_settings_path() {
    return FileManager::append(FileManager::get_app_directory(), "settings.txt");
}

/*!
 * \brief Get the database file path
 * \return The database file path
 */
std::string FileManager::get_database_path() {
    return FileManager::append(FileManager::get_app_directory(), "database.sqlite3");
}

/*!
 * \brief Get a class's folder path
 * \param class_ The class
 * \return The class's folder path
 */
std::string FileManager::get_class_directory(GAClass* class_) {
    std::string directory = FileManager::get_app_directory();
    std::string classID = class_->get_id();
    std::string classPath = FileManager::append(directory, "assignment-data", "class-" + classID);
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
    std::string assignmentPath = FileManager::append(classDirectory, "assignment-" + assignmentID);
    return assignmentPath;
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
std::vector<std::string> FileManager::import(std::string path, GradingAssistant* ga, GAAssignment* assign) {
    std::vector<std::string> made_ids;
    QDir importDir(QString::fromStdString(path));
    importDir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot | QDir::NoSymLinks);
    QDirIterator it(importDir, QDirIterator::NoIteratorFlags);
    while (it.hasNext()) {
        QString path = it.next();
        QDir studentDir(path);
        std::string expected_lafid = studentDir.dirName().toStdString();
        GAStudent* student = ga->get_student(expected_lafid);
        if (student == nullptr) {
            if (expected_lafid.find(" ") != std::string::npos) {
                std::cout << "Not making " << expected_lafid << std::endl;
                continue;
            }
            student = new GAStudent(ga);
            assign->get_class()->add_student(student);

            student->set_lafayette_username(expected_lafid);
            student->set_name("No Name (" + expected_lafid + ")");

            student->save(true);

            made_ids.push_back(expected_lafid);
        }
        std::string intendedPath = FileManager::get_assignment_student_directory(assign, student);
        QDir intendedDir(QString::fromStdString(intendedPath));
        intendedDir.removeRecursively();
        FileManager::assure_directory_exists(FileManager::get_assignment_directory(assign));
        FileManager::copy_directory(studentDir.absolutePath().toStdString(), intendedDir.absolutePath().toStdString());
    }

    return made_ids;
}

/*!
 * \brief Get a list of files in the directory
 * \param path The directory
 */
std::vector<std::pair<std::string, std::string>> FileManager::get_files_in(std::string path) {
    std::vector<std::pair<std::string, std::string>> ret;
    QDir root(QString::fromStdString(path));
    root.setFilter(QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks);
    QDirIterator it(root, QDirIterator::Subdirectories);
    while (it.hasNext()) {
        QString path = it.next();
        QFileInfo fi(path);
        std::pair<std::string, std::string> item;
        item.first = fi.fileName().toStdString();
        item.second = path.toStdString();
        ret.push_back(item);
    }
    return ret;
}

/*!
 * \brief Copy a directory at p1 to p2
 * \param p1 The path to the directory
 * \param p2 The new path to the directory
 */
void FileManager::copy_directory(std::string p1, std::string p2) {
    QDir dir(QString::fromStdString(p1));

    QDir target(QString::fromStdString(p2));
    !target.removeRecursively();
    FileManager::assure_directory_exists(p2);

    for(QString found: dir.entryList(QDir::Files)) {
        QString fileTo = QString::fromStdString(FileManager::append(p2, found.toStdString()));
        if (QFile::exists(fileTo) && !QFile::remove(fileTo)) {
            std::cout << "Skipped " << fileTo.toStdString() << ": conflict at destination" << std::endl;
            continue;
        }
        QFile::copy(QString::fromStdString(FileManager::append(p1, found.toStdString())), fileTo);
    }

    for(QString found: dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot)) {
        FileManager::copy_directory(FileManager::append(p1, found.toStdString()), FileManager::append(p2, found.toStdString()));
    }
}
