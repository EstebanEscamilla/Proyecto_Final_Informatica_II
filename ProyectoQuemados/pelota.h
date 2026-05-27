#ifndef PELOTA_H
#define PELOTA_H
#include "FisicaObjeto.h"

class Pelota : public FisicaObjeto {
private:
    float velX;
    float velY;
    float elasticidad;
    const float GRAVEDAD = 9.8f; // Puedes ajustar este valor para que el juego se sienta mejor

public:
    Pelota(float x, float y, int w, int h, float e)
        : FisicaObjeto(x, y, w, h), velX(0), velY(0), elasticidad(e) {}

    void lanzar(float v0, float anguloRadianes);
    void rebotar(bool colisionVertical);

    // Implementación obligatoria de las clases abstractas
    void actualizar(float dt) override;
    void aplicarFisica(float dt) override;
};

#endif // PELOTA_H
