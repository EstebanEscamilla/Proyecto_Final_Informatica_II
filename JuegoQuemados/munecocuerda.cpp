#include "munecocuerda.h"

MunecoCuerda::MunecoCuerda(float x, float y, int ancho, int alto)
    : FisicaObjeto(x, y, ancho, alto) {
    tieneColision = true;
    salud = 5; // El jefe necesita 5 pelotazos de la dodgeball para ser derrotado
    velocidadMovimiento = 80.0f; // Un poco más lento que Timmy para que sea un reto justo
    estadoActual = PERSEGUIR;
    tiempoAtaque = 0.0f;
    tiempoEnfriamiento = 0.0f;
}

void MunecoCuerda::aplicarFisica(float dt) {
    // Como el jefe vive en el Nivel 2 (Vista Lateral), debe estar afectado por la gravedad
    this->posY += 300.0f * dt;

    // Piso del Nivel 2 (Matemáticamente el mismo piso de Timmy)
    if (this->posY >= 480.0f) {
        this->posY = 480.0f;
    }
}

void MunecoCuerda::actualizar(float dt) {
    // Reservado para cambiar los sprites (animar) dependiendo del estado del jefe
}

void MunecoCuerda::recibirDano() {
    // Solo puede recibir daño si no está ya en su periodo de invulnerabilidad (aturdido)
    if (estadoActual != ENFRIAMIENTO) {
        salud--;
        estadoActual = ENFRIAMIENTO;
        tiempoEnfriamiento = 0.0f; // Inicia el reloj de aturdimiento
    }
}

void MunecoCuerda::pensar(float dt, float timmyX, float timmyY) {
    // Calculamos la distancia absoluta en X entre el Jefe y Timmy
    float distanciaX = std::abs(this->posX - timmyX);

    switch (estadoActual) {
    case PERSEGUIR:
        // IA Básica de Seguimiento
        if (timmyX > this->posX) {
            this->posX += velocidadMovimiento * dt; // Timmy está a la derecha
        }
        else if (timmyX < this->posX) {
            this->posX -= velocidadMovimiento * dt; // Timmy está a la izquierda
        }

        // Si está muy cerca (Hitbox de ataque: 80 píxeles), se prepara para atacar
        if (distanciaX < 80.0f) {
            estadoActual = ATACAR;
            tiempoAtaque = 0.0f;
        }
        break;

    case ATACAR:
        // El jefe se detiene e incrementa su reloj de ataque
        tiempoAtaque += dt;

        // Si Timmy es rápido y se aleja de su rango, el jefe cancela el ataque y lo vuelve a perseguir
        if (distanciaX > 100.0f) {
            estadoActual = PERSEGUIR;
        }
        break;

    case ENFRIAMIENTO:
        // El jefe recibió un pelotazo. Se queda completamente quieto (aturdido).
        tiempoEnfriamiento += dt;

        // Después de 1 segundo de invulnerabilidad temporal, vuelve al combate
        if (tiempoEnfriamiento >= 1.0f) {
            estadoActual = PERSEGUIR;
        }
        break;
    }
}
