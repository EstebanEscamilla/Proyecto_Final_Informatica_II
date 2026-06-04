#include "munecocuerda.h"

MunecoCuerda::MunecoCuerda(float x, float y, int ancho, int alto)
    : FisicaObjeto(x, y, ancho, alto) {
    tieneColision = true;
    salud = 3;
    velocidadMovimiento = 150.0f;
    velocidadY = 0.0f;
    direccionX = -1;
    estadoActual = PATRULLAR;
    tiempoEnfriamiento = 0.0f;
    tiempoSalto = 0.0f;

    // VARIABLES DEL CAÑÓN
    tiempoDisparo = 2.0f;
    lanzarProyectil = false;
}

void MunecoCuerda::aplicarFisica(float dt) {
    velocidadY += 900.0f * dt;
    this->posY += velocidadY * dt;

    if (this->posY >= 480.0f) {
        this->posY = 480.0f;
        velocidadY = 0.0f;
    }
}

void MunecoCuerda::actualizar(float dt) {
}

void MunecoCuerda::recibirDano() {
    if (estadoActual != ENFRIAMIENTO) {
        salud--;
        estadoActual = ENFRIAMIENTO;
        tiempoEnfriamiento = 0.0f;
    }
}

void MunecoCuerda::pensar(float dt, float pelotaX, float pelotaY) {
    if (tiempoSalto > 0.0f) tiempoSalto -= dt;

    switch (estadoActual) {
    case PATRULLAR:
        // IA EVASIVA: El jefe mira dónde está la pelota en el aire
        if (pelotaX > 300.0f && pelotaX < 780.0f) { // Si el balón entró a su zona

            // 1. Si el balón viene ALTO, el jefe intenta retroceder (huir a la derecha)
            if (pelotaY < 350.0f) {
                direccionX = 1;
            }
            // 2. Si el balón viene BAJO
            else if (pelotaY >= 350.0f && this->posY >= 480.0f && tiempoSalto <= 0.0f) {
                velocidadY = -650.0f; // Salto fuerte
                tiempoSalto = 1.2f;   // No puede spamear el salto
            }

            // Aplica la evasión (Se mueve más rápido si está esquivando)
            this->posX += (velocidadMovimiento * 1.2f * direccionX) * dt;

        } else {
            this->posX += (velocidadMovimiento * 0.6f * direccionX) * dt;
            if (this->posX >= 650.0f) direccionX = -1;
            if (this->posX <= 500.0f) direccionX = 1;
        }

        // Seguro para no ir por fuera de la pantalla
        if (this->posX > 680.0f) this->posX = 680.0f;
        if (this->posX < 450.0f) this->posX = 450.0f;

        // RELOJ DE DISPARO DEL JEFE
        if (tiempoDisparo > 0.0f) tiempoDisparo -= dt;

        if (tiempoDisparo <= 0.0f) {
            lanzarProyectil = true;
            tiempoDisparo = 1.0f + static_cast<float>(rand() % 2);
        }
        break;

    case ENFRIAMIENTO:
        tiempoEnfriamiento += dt;
        if (tiempoEnfriamiento >= 1.0f) estadoActual = PATRULLAR;
        break;
    }
}
