#ifndef MUNECOCUERDA_H
#define MUNECOCUERDA_H

#include "fisicaobjeto.h"
#include <cmath>

enum EstadoJefe {
    PATRULLAR,
    ENFRIAMIENTO
};

class MunecoCuerda : public FisicaObjeto {
private:
    int salud;
    float velocidadMovimiento;
    float velocidadY;
    int direccionX;
    EstadoJefe estadoActual;

    float tiempoEnfriamiento;
    float tiempoSalto;

    float tiempoDisparo;
    bool lanzarProyectil;

public:
    MunecoCuerda(float x, float y, int ancho, int alto);

    void aplicarFisica(float dt) override;
    void actualizar(float dt) override;
    void recibirDano();

    int getSalud() const { return salud; }
    EstadoJefe getEstado() const { return estadoActual; }

    bool getLanzarProyectil() const { return lanzarProyectil; }
    void setLanzarProyectil(bool estado) { lanzarProyectil = estado; }

    void pensar(float dt, float pelotaX, float pelotaY);
};

#endif // MUNECOCUERDA_H
