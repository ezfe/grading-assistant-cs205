#include "gaidentifiableobject.h"

GAIdentifiableObject::GAIdentifiableObject() {
    this->id = QUuid::createUuid().toString().toStdString();
}

GAIdentifiableObject::GAIdentifiableObject(std::string id) {
    this->id = id;
}

GAIdentifiableObject::~GAIdentifiableObject() {

}

std::string GAIdentifiableObject::get_id() {
    return this->id;
}

void GAIdentifiableObject::set_id(std::string id) {
    this->id = id;
}
