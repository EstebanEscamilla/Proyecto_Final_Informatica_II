#include "fisicaobjeto.h"

FisicaObjeto::FisicaObjeto(float x, float y, int w, int h) : Entidad(x, y, w, h) {
    this->tieneColision = true;
}
