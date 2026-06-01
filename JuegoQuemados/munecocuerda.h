#ifndef MUNECOCUERDA_H
#define MUNECOCUERDA_H

#include "fisicaobjeto.h"
#include <cmath> // Para calcular distancias

// Enumerador para la Máquina de Estados
enum EstadoJefe {
    PERSEGUIR,
    ATACAR,
    ENFRIAMIENTO
};

class MunecoCuerda : public FisicaObjeto {
private:
    int salud;
    float velocidadMovimiento;
    EstadoJefe estadoActual;

    // Temporizadores internos
    float tiempoAtaque;
    float tiempoEnfriamiento;

public:
    MunecoCuerda(float x, float y, int ancho, int alto);

    // Heredados de FisicaObjeto
    void aplicarFisica(float dt) override;
    void actualizar(float dt) override; // Aquí meteremos la IA

    // Acciones del Jefe
    void recibirDano();
    int getSalud() const { return salud; }
    EstadoJefe getEstado() const { return estadoActual; }

    // La función principal de su IA
    void pensar(float dt, float timmyX, float timmyY);
};

#endif // MUNECOCUERDA_H
