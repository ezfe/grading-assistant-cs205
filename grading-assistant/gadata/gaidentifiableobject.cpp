#include "gaidentifiableobject.h"

/*!
 * \brief Create with a random ID
 *
 * If loading an object from the database, be sure to call set_id(std::string id) afterwards
 */
GAIdentifiableObject::GAIdentifiableObject() {
    this->id = QUuid::createUuid().toString().toStdString();
//    std::cout << "Created new ID" << std::endl;
}

/*!
 * \brief Create a new object with a provided ID
 * \param id The ID to use
 */
GAIdentifiableObject::GAIdentifiableObject(std::string id) {
    this->id = id;
}

/*!
 * \brief Create with a grading assistant and random ID
 * \param ga The grading assistant
 */
GAIdentifiableObject::GAIdentifiableObject(GradingAssistant* ga) {
    this->id = QUuid::createUuid().toString().toStdString();
    this->set_grading_assistant(ga);
}

/*!
 * \brief Deconstruct the GAIdentiableObject
 */
GAIdentifiableObject::~GAIdentifiableObject() {

}

/*!
 * \brief Get the ID of the object
 * \return The ID of the object. Will always be 38 characters
 */
std::string GAIdentifiableObject::get_id() {
    std::string proposed = this->id;
    if (proposed.at(0) != '{' || proposed.at(37) != '}' || proposed.size() != 38) {
        // proposed isn't ok, return blank id
        return QUuid().toString().toStdString();
    }
    return proposed;
}

/*!
 * \brief Set the ID of the object
 * \param id The new ID
 */
void GAIdentifiableObject::set_id(std::string id) {
    this->id = id;
    std::cout << "Manually updated an ID" << std::endl;
}

/*!
 * \brief Get the grading assistant
 * \return The grading assistant
 */
GradingAssistant* GAIdentifiableObject::get_grading_assistant() {
    if (this->grading_assistant == nullptr) {
        std::cerr << " * There is no grading assistant" << std::endl;
        std::cerr << "   This may not be a problem if you are setting the grading assistant before" << std::endl;
        std::cerr << "   adding constituent objects or saving" << std::endl;
    }
    return this->grading_assistant;
}

/*!
 * \brief Set the grading assistant
 * \param ga The grading assistant
 */
void GAIdentifiableObject::set_grading_assistant(GradingAssistant* ga) {
    if (ga == nullptr) {
        std::cerr << "The passed assistant is a null pointer" << std::endl;
        std::cerr << "This is a problem because data structures are not recursively self correcting" << std::endl;
        std::cerr << "IF YOU ARE GETTING THIS ERROR YOU SHOULD FIX IT" << std::endl;
        return;
    }
    this->grading_assistant = ga;
}
