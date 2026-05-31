#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsRectItem>
#include <QGraphicsPixmapItem>
#include <QGraphicsTextItem> // NUEVO: Para los textos en pantalla
#include <QFont>             // NUEVO: Para cambiar el tipo de letra
#include <QTimer>
#include <QKeyEvent>
#include <vector>

#include "jugador.h"
#include "pelota.h"
#include "osopeluche.h"
#include "botonafilado.h"
#include "pendulo.h"

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

    // Fondos móviles y filtros
    QGraphicsPixmapItem *fondoCinta1;
    QGraphicsPixmapItem *fondoCinta2;
    QGraphicsRectItem *filtroOscuro;
    float velCinta;

    // Protagonista y proyectil principal
    Jugador *timmy;
    Pelota *miPelota;
    QGraphicsPixmapItem *graficoTimmy;
    QGraphicsPixmapItem *graficoPelota;

    // Enemigos y obstáculos
    OsoPeluche *osoTest;
    QGraphicsPixmapItem *graficoOsoTest;
    Pendulo *gancho;
    QGraphicsPixmapItem *graficoGancho;

    // Proyectiles enemigos
    std::vector<BotonAfilado*> listaBotones;
    std::vector<QGraphicsPixmapItem*> listaGraficosBotones;

    // NUEVO: Interfaz de Usuario (HUD) y Lógica de Progreso
    QGraphicsTextItem *textoVidas;
    QGraphicsTextItem *textoTiempo;
    int framesSobrevividos; // Contador interno para medir los segundos
    void gameOver();        // Función para terminar la partida

    // Interruptores de estado
    bool teclaIzquierda;
    bool teclaDerecha;
    bool teclaArriba;
    bool teclaAbajo;
    bool pelotaEnMano;
    int nivelActual;

    // Funciones auxiliares
    void cargarNivel(int numeroNivel);
    void limpiarBotones();
};

#endif // MAINWINDOW_H
