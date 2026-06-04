#include "osopeluche.h"

OsoPeluche::OsoPeluche(float x, float y, int ancho, int alto)
    : FisicaObjeto(x, y, ancho, alto) {
    tieneColision = true;
    tiempoDisparo = 0.0f;
    cadenciaDisparo = 2.0f; // Está configurado para disparar un botón cada 2 segundos
}

void OsoPeluche::aplicarFisica(float dt) {
}

void OsoPeluche::actualizar(float dt) {
}

BotonAfilado* OsoPeluche::disparar(float dt, float dirX, float dirY) {
    tiempoDisparo += dt;

    // Si el reloj interno supera la cadencia, genera un disparo
    if (tiempoDisparo >= cadenciaDisparo) {
        tiempoDisparo = 0.0f; // Reiniciamos el reloj para el próximo disparo

        // Creamos y disparamos el boton (nace un poco desplazado hacia el centro del oso)
        return new BotonAfilado(this->posX + 30.0f, this->posY + 80.0f, 40, 40, dirX, dirY);
    }

    // Si aún no es tiempo, no retorna nada
    return nullptr;
}
