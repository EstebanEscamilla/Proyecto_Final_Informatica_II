#include "pendulo.h"

Pendulo::Pendulo(float x, float y, int ancho, int alto, float amp, float omega)
    : FisicaObjeto(x, y, ancho, alto), xCentro(x), amplitud(amp), velocidadAngular(omega) {
    tieneColision = true;
    tiempoOscilacion = 0.0f; // Inicia en t=0
}

void Pendulo::aplicarFisica(float dt) {
    // El tiempo interno avanza
    tiempoOscilacion += dt;

    // Aplicamos la fórmula del M.A.S: x(t) = X0 + A * cos(w * t)
    // std::cos requiere la librería <cmath> que ya incluimos en el .h
    this->posX = xCentro + amplitud * std::cos(velocidadAngular * tiempoOscilacion);

    // El eje Y se mantiene estático, ya que el gancho solo se mueve de izquierda a derecha
}

void Pendulo::actualizar(float dt) {
    // Reservado para futuras animaciones del sprite
}
