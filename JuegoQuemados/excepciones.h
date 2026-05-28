#ifndef EXCEPCIONES_H
#define EXCEPCIONES_H

#include <string>
#include <exception>

class ExcepcionFuerzaInvalida : public std::exception {
private:
    std::string mensajeError;
public:
    ExcepcionFuerzaInvalida(std::string msj);
    std::string getMensaje() const;
    const char* what() const noexcept override;
};

class ExcepcionFisicaFueraDeLimites : public std::exception {
private:
    std::string mensajeError;
public:
    ExcepcionFisicaFueraDeLimites(std::string msj);
    std::string getMensaje() const;
    const char* what() const noexcept override;
};

#endif // EXCEPCIONES_H
