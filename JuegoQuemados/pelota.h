#ifndef PELOTA_H
#define PELOTA_H
#include "fisicaobjeto.h"

class Pelota : public FisicaObjeto {
private:
    float velX;
    float velY;
    float elasticidad;
    const float GRAVEDAD = 9.8f;

public:
    Pelota(float x, float y, int w, int h, float e);

    void lanzar(float v0, float anguloRadianes);
    void rebotar(bool colisionVertical);

    void actualizar(float dt) override;
    void aplicarFisica(float dt) override;

    void rebotarHorizontal();
    void rebotarVertical();
};

#endif // PELOTA_H
