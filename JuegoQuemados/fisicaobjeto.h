#ifndef FISICAOBJETO_H
#define FISICAOBJETO_H
#include "entidad.h"

class FisicaObjeto : public Entidad {
protected:
    bool tieneColision;

public:
    FisicaObjeto(float x, float y, int w, int h);
    virtual void aplicarFisica(float dt) = 0;

    void setPosicion(float x, float y) {
        this->posX = x;
        this->posY = y;
    }
};

#endif // FISICAOBJETO_H
