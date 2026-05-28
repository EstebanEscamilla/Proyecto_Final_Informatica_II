#include "entidad.h"

Entidad::Entidad(float x, float y, int w, int h) {
    this->posX = x;
    this->posY = y;
    this->ancho = w;
    this->alto = h;
    this->activa = true;
}

float Entidad::getPosX() const {
    return this->posX;
}

float Entidad::getPosY() const {
    return this->posY;
}

bool Entidad::getActiva() const {
    return this->activa;
}
