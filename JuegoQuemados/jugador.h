#ifndef JUGADOR_H
#define JUGADOR_H

#include "fisicaobjeto.h"
#include <vector>
#include <cmath>

class Jugador : public FisicaObjeto {
private:
    float velocidadBase;
    float velocidadActual;
    float fuerzaCarga;
    float fuerzaMaxima;
    bool estaCargando;
    bool estaAsustado;
    int vidas;

    float radioTerror;
    float factorAdrenalina;

public:
    Jugador(float x, float y, int ancho, int alto);

    void mover(float dx, float dy);

    void iniciarCarga();
    float soltarCarga();

    // Cumpliendo todos los contratos de herencia
    void actualizar(float dt) override;
    void aplicarFisica(float dt) override; // ¡El contrato que faltaba!

    void actualizarEstado(const std::vector<FisicaObjeto*>& enemigos);

    float getFuerzaCarga() const { return fuerzaCarga; }
    bool getEstaAsustado() const { return estaAsustado; }
    int getVidas() const { return vidas; }
    void reducirVida() { if(vidas > 0) vidas--; }
};

#endif // JUGADOR_H
