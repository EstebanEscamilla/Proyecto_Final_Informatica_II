#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsRectItem>
#include <QGraphicsPixmapItem>
#include <QGraphicsTextItem>
#include <QFont>
#include <QTimer>
#include <QKeyEvent>
#include <vector>

#include "jugador.h"
#include "pelota.h"
#include "osopeluche.h"
#include "botonafilado.h"
#include "pendulo.h"
#include "munecocuerda.h" // <-- CRUCIAL: Aquí le enseñamos a la ventana qué es un MunecoCuerda

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

private slots:
    void actualizarJuego();

private:
    Ui::MainWindow *ui;
    QGraphicsScene *escena;
    QGraphicsView *vista;
    QTimer *temporizador;

    // Límites visuales
    QGraphicsRectItem *techo;
    QGraphicsRectItem *piso;
    QGraphicsRectItem *paredIzquierda;
    QGraphicsRectItem *paredDerecha;

    // Fondos móviles y estáticos
    QGraphicsPixmapItem *fondoCinta1;
    QGraphicsPixmapItem *fondoCinta2;
    QGraphicsRectItem *filtroOscuro;
    QGraphicsPixmapItem *fondoFijoNivel2; // Fondo del almacén
    float velCinta;

    // Protagonista y proyectil principal
    Jugador *timmy;
    Pelota *miPelota;
    QGraphicsPixmapItem *graficoTimmy;
    QGraphicsPixmapItem *graficoPelota;

    // Enemigos y obstáculos
    OsoPeluche *osoTest;
    QGraphicsPixmapItem *graficoOsoTest;
    Pendulo *gancho; // Ahora disfrazado de caja embrujada
    QGraphicsPixmapItem *graficoGancho;

    // ==========================================
    // ¡AQUÍ ESTÁ LA SOLUCIÓN A TU ERROR!
    // ==========================================
    MunecoCuerda *jefeFinal;
    QGraphicsPixmapItem *graficoJefe;

    // Proyectiles enemigos
    std::vector<BotonAfilado*> listaBotones;
    std::vector<QGraphicsPixmapItem*> listaGraficosBotones;

    // Interfaz de Usuario (HUD) Actualizada
    std::vector<QGraphicsPixmapItem*> iconosVidas; // Vector de corazones
    QGraphicsTextItem *textoTiempo;
    int framesSobrevividos;

    void gameOver();
    void victoria();

    // Interruptores de estado
    bool teclaIzquierda;
    bool teclaDerecha;
    bool teclaArriba;
    bool teclaAbajo;
    bool pelotaEnMano;
    int nivelActual;

    // Interruptores de estado
    int direccionOso; // 1 para derecha, -1 para izquierda

    // Variables para el Bonus del Nivel 1
    QGraphicsPixmapItem *graficoBonus;
    bool bonusActivo;
    float tiempoParaBonus;

    // Funciones auxiliares
    void cargarNivel(int numeroNivel);
    void limpiarBotones();
};

#endif // MAINWINDOW_H
