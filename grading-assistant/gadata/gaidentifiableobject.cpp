#include "gaidentifiableobject.h"

GAIdentifiableObject::GAIdentifiableObject() {
    this->id = QUuid::createUuid().toString().toStdString();
}

GAIdentifiableObject::~GAIdentifiableObject() {

}

std::string GAIdentifiableObject::id_string() {
    return this->id;
}
