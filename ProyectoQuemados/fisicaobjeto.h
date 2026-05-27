#ifndef FISICAOBJETO_H
#define FISICAOBJETO_H
#include "Entidad.h"

class FisicaObjeto : public Entidad {
protected:
    bool tieneColision;

public:
    FisicaObjeto(float x, float y, int w, int h) : Entidad(x, y, w, h), tieneColision(true) {}

    // Método virtual puro para obligar a implementar físicas específicas
    virtual void aplicarFisica(float dt) = 0;
};

#endif // FISICAOBJETO_H
