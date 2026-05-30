#include "pelota.h"
#include "excepciones.h"
#include <cmath>

Pelota::Pelota(float x, float y, int w, int h, float e)
    : FisicaObjeto(x, y, w, h), velX(0), velY(0), elasticidad(e) {}

void Pelota::lanzar(float v0, float anguloRadianes) {
    if (v0 < 0.0f) {
        throw ExcepcionFuerzaInvalida("Error: La fuerza de carga inicial no puede ser negativa.");
    }

    this->velX = v0 * std::cos(anguloRadianes);
    this->velY = -v0 * std::sin(anguloRadianes);
}

void Pelota::rebotar(bool colisionVertical) {
    if (colisionVertical) {
        this->velY = -this->velY * this->elasticidad;
    } else {
        this->velX = -this->velX * this->elasticidad;
    }
}

void Pelota::aplicarFisica(float dt) {
    this->velY += GRAVEDAD * dt;
    this->posX += this->velX * dt;
    this->posY += this->velY * dt;

    if (this->posX < -5000.0f || this->posX > 5000.0f || this->posY > 5000.0f) {
        this->activa = false;
        throw ExcepcionFisicaFueraDeLimites("Error Critico: La pelota salio de los limites.");
    }
}

void Pelota::actualizar(float dt) {
    if (this->activa) {
        aplicarFisica(dt);
    }
}

void Pelota::rebotarHorizontal() {
    this->velX = -this->velX * this->elasticidad;
}

void Pelota::rebotarVertical() {
    this->velY = -std::abs(this->velY) * this->elasticidad;
}
