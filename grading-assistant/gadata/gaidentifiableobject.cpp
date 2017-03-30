#include "gaidentifiableobject.h"

GAIdentifiableObject::GAIdentifiableObject() {
    std::cerr << "ID must be passed at init-time" << std::endl;
}

GAIdentifiableObject::GAIdentifiableObject(int id) {
    this->id = id;
}

std::string GAIdentifiableObject::id_string() {
    return std::to_string(this->id);
}
