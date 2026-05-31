#include "botonafilado.h"

BotonAfilado::BotonAfilado(float x, float y, int ancho, int alto, float velX, float velY)
    : FisicaObjeto(x, y, ancho, alto), velocidadX(velX), velocidadY(velY) {
    tieneColision = true;
}

void BotonAfilado::aplicarFisica(float dt) {
    // Aplicamos Movimiento Rectilíneo Uniforme (MRU) sin gravedad
    this->posX += velocidadX * dt;
    this->posY += velocidadY * dt;
}

void BotonAfilado::actualizar(float dt) {
    // La actualización matemática directa
}
