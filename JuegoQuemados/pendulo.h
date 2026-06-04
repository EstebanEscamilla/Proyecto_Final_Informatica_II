#ifndef PENDULO_H
#define PENDULO_H

#include "fisicaobjeto.h"
#include <cmath>

class Pendulo : public FisicaObjeto {
private:
    float xCentro;
    float amplitud;
    float velocidadAngular;
    float tiempoOscilacion;

public:
    Pendulo(float x, float y, int ancho, int alto, float amp, float omega);

    // Contratos heredados
    void aplicarFisica(float dt) override;
    void actualizar(float dt) override;
};

#endif // PENDULO_H
