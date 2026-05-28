#ifndef ENTIDAD_H
#define ENTIDAD_H

class Entidad {
protected:
    float posX;
    float posY;
    int ancho;
    int alto;
    bool activa;

public:
    Entidad(float x, float y, int w, int h);
    virtual ~Entidad() = default;

    virtual void actualizar(float dt) = 0;

    float getPosX() const;
    float getPosY() const;
    bool getActiva() const;
};

#endif // ENTIDAD_H
