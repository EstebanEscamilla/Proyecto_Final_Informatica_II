#include "pendulo.h"

Pendulo::Pendulo(float x, float y, int ancho, int alto, float amp, float omega)
    : FisicaObjeto(x, y, ancho, alto), xCentro(x), amplitud(amp), velocidadAngular(omega) {
    tieneColision = true;
    tiempoOscilacion = 0.0f;
}

void Pendulo::aplicarFisica(float dt) {
    // El tiempo interno avanza
    tiempoOscilacion += dt;

    // Aplicamos la fórmula del M.A.S: x(t) = X0 + A * cos(w * t)
    this->posX = xCentro + amplitud * std::cos(velocidadAngular * tiempoOscilacion);

}

void Pendulo::actualizar(float dt) {

}
