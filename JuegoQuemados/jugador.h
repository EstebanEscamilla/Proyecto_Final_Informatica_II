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

    //Variables para la física del salto y gravedad
    float velY;
    float gravedad;
    float fuerzaSalto;
    bool enSuelo;
    bool gravedadActiva;

    float radioTerror;
    float factorAdrenalina;

public:
    Jugador(float x, float y, int ancho, int alto);

    void mover(float dx, float dy);

    void iniciarCarga();
    float soltarCarga();

    // Métodos de control físico
    void saltar();
    void setGravedadActiva(bool activa) { gravedadActiva = activa; }
    void setEnSuelo(bool suelo) { enSuelo = suelo; if(suelo) velY = 0.0f; }

    // todos los contratos de herencia
    void actualizar(float dt) override;
    void aplicarFisica(float dt) override;

    void actualizarEstado(const std::vector<FisicaObjeto*>& enemigos);

    float getFuerzaCarga() const { return fuerzaCarga; }
    bool getEstaAsustado() const { return estaAsustado; }
    int getVidas() const { return vidas; }
    void reducirVida() { if(vidas > 0) vidas--; }
};

#endif // JUGADOR_H
