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
    return this->grading_assistant;
}

/*!
 * \brief Set the grading assistant
 * \param ga The grading assistant
 */
void GAIdentifiableObject::set_grading_assistant(GradingAssistant* ga) {
    this->grading_assistant = ga;
}
