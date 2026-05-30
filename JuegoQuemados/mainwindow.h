#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsEllipseItem>
#include <QTimer>
#include "pelota.h" // Tu clase de física
#include "jugador.h"
#include <QGraphicsRectItem>
#include <QKeyEvent>
#include <QGraphicsPixmapItem>
#include <QPixmap>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void actualizarJuego(); // El "corazón" del juego cuadro por cuadro

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

private:
    Ui::MainWindow *ui;

    // Herramientas de Qt para gráficos
    QGraphicsScene *escena;
    QGraphicsView *vista;
    QTimer *temporizador;
    bool teclaIzquierda;
    bool teclaDerecha;

    // Conexión con tus clases
    Pelota *miPelota;                         // Objeto físico
    Jugador *timmy;
    bool pelotaEnMano;

    QGraphicsRectItem *piso;
    QGraphicsRectItem *paredIzquierda;
    QGraphicsRectItem *paredDerecha;
    QGraphicsRectItem *techo;

    //gráficos geométricos por contenedores de Sprites
    QGraphicsPixmapItem *graficoTimmy;
    QGraphicsPixmapItem *graficoPelota;

    int nivelActual;
    bool teclaArriba;
    bool teclaAbajo;

    void cargarNivel(int numeroNivel);


};

#endif // MAINWINDOW_H
