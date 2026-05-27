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
    Entidad(float x, float y, int w, int h) : posX(x), posY(y), ancho(w), alto(h), activa(true) {}
    virtual ~Entidad() = default;

    // Método virtual puro: hace de esta clase una abstracta
    virtual void actualizar(float dt) = 0;

    // Getters
    float getPosX() const { return posX; }
    float getPosY() const { return posY; }
    bool getActiva() const { return activa; }
};

#endif // ENTIDAD_H
