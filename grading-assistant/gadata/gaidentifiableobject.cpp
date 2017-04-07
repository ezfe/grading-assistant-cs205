#include "gaidentifiableobject.h"

GAIdentifiableObject::GAIdentifiableObject() {
    this->id = QUuid::createUuid().toString().toStdString();
    std::cout << "Created new ID! This shouldn't happen when loading from database." << std::endl;
}

GAIdentifiableObject::GAIdentifiableObject(std::string id) {
    this->id = id;
}

GAIdentifiableObject::~GAIdentifiableObject() {

}

std::string GAIdentifiableObject::get_id() {
    std::string proposed = this->id;
    if (proposed.at(0) != '{' || proposed.at(37) != '}') {
        // proposed isn't ok, return blank id
        return QUuid().toString().toStdString();
    }
    return proposed;
}

void GAIdentifiableObject::set_id(std::string id) {
    this->id = id;
}
