#include "gaidentifiableobject.h"

GAIdentifiableObject::GAIdentifiableObject() {
    this->id = QUuid::createUuid().toString().toStdString();
    std::cout << "Init: " << this->id_string() << std::endl;
}

GAIdentifiableObject::~GAIdentifiableObject() {

}

std::string GAIdentifiableObject::id_string() {
    return this->id;
}
