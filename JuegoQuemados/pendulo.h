#ifndef PENDULO_H
#define PENDULO_H

#include "fisicaobjeto.h"
#include <cmath>

class Pendulo : public FisicaObjeto {
private:
    float xCentro;           // El punto medio (X0) de la oscilación
    float amplitud;          // Distancia máxima de recorrido (A)
    float velocidadAngular;  // Frecuencia de oscilación (Omega)
    float tiempoOscilacion;  // El reloj interno del M.A.S (t)

public:
    Pendulo(float x, float y, int ancho, int alto, float amp, float omega);

    // Contratos heredados
    void aplicarFisica(float dt) override;
    void actualizar(float dt) override;
};

#endif // PENDULO_H
