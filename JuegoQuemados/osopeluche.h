#ifndef OSOPELUCHE_H
#define OSOPELUCHE_H

#include "fisicaobjeto.h"
#include "botonafilado.h"

class OsoPeluche : public FisicaObjeto {
private:
    float tiempoDisparo;
    float cadenciaDisparo;

public:
    OsoPeluche(float x, float y, int ancho, int alto);

    void aplicarFisica(float dt) override;
    void actualizar(float dt) override;

    // El "cerebro" balístico del oso
    BotonAfilado* disparar(float dt, float dirX, float dirY);
};

#endif // OSOPELUCHE_H
