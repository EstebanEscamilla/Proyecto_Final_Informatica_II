#ifndef BOTONAFILADO_H
#define BOTONAFILADO_H

#include "fisicaobjeto.h"

class BotonAfilado : public FisicaObjeto {
private:
    float velocidadX;
    float velocidadY;

public:
    BotonAfilado(float x, float y, int ancho, int alto, float velX, float velY);

    // Contratos heredados de FisicaObjeto
    void aplicarFisica(float dt) override;
    void actualizar(float dt) override;
};

#endif // BOTONAFILADO_H
