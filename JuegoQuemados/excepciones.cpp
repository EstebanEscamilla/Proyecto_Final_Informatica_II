#include "excepciones.h"

ExcepcionFuerzaInvalida::ExcepcionFuerzaInvalida(std::string msj) {
    this->mensajeError = msj;
}

std::string ExcepcionFuerzaInvalida::getMensaje() const {
    return this->mensajeError;
}

const char* ExcepcionFuerzaInvalida::what() const noexcept {
    return this->mensajeError.c_str();
}

ExcepcionFisicaFueraDeLimites::ExcepcionFisicaFueraDeLimites(std::string msj) {
    this->mensajeError = msj;
}

std::string ExcepcionFisicaFueraDeLimites::getMensaje() const {
    return this->mensajeError;
}

const char* ExcepcionFisicaFueraDeLimites::what() const noexcept {
    return this->mensajeError.c_str();
}
