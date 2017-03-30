#include "gaidentifiableobject.h"

GAIdentifiableObject::GAIdentifiableObject() {
    this->id = QUuid::createUuid().toString().toStdString();
    std::cout << this->id << std::endl;
}

std::string GAIdentifiableObject::id_string() {
    return this->id;
}
