#include "jugador.h"

Jugador::Jugador(float x, float y, int ancho, int alto)
    : FisicaObjeto(x, y, ancho, alto) {

    velocidadBase = 5.0f;
    velocidadActual = velocidadBase;
    fuerzaCarga = 0.0f;
    fuerzaMaxima = 150.0f;
    estaCargando = false;
    estaAsustado = false;
    vidas = 3;
    radioTerror = 150.0f;
    factorAdrenalina = 1.2f;
}

void Jugador::mover(float dx, float dy) {
    this->posX += dx * velocidadActual;
    this->posY += dy * velocidadActual;
}

void Jugador::iniciarCarga() {
    estaCargando = true;
    fuerzaCarga = 0.0f;
}

float Jugador::soltarCarga() {
    estaCargando = false;
    float fuerzaFinal = fuerzaCarga;
    fuerzaCarga = 0.0f;
    return fuerzaFinal;
}

void Jugador::actualizar(float dt) {
    // La mantenemos por si la clase Entidad la exige
}

void Jugador::aplicarFisica(float dt) {
    // ¡Aquí calculamos la fuerza de los Brazos de Fideo!
    if (estaCargando) {
        if (fuerzaCarga < fuerzaMaxima) {
            fuerzaCarga += 100.0f * dt;
        }
    }
}

void Jugador::actualizarEstado(const std::vector<FisicaObjeto*>& enemigos) {
    estaAsustado = false;
    for (auto enemigo : enemigos) {
        float dx = enemigo->getPosX() - this->posX;
        float dy = enemigo->getPosY() - this->posY;
        float distancia = std::sqrt(dx*dx + dy*dy);

        if (distancia < radioTerror) {
            estaAsustado = true;
            break;
        }
    }

    if (estaAsustado) {
        velocidadActual = velocidadBase * factorAdrenalina;
    } else {
        velocidadActual = velocidadBase;
    }
}
