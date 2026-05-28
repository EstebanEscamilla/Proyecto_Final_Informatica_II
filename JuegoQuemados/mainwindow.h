#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsEllipseItem>
#include <QTimer>
#include "pelota.h" // Tu clase de física

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

private:
    Ui::MainWindow *ui;

    // Herramientas de Qt para gráficos
    QGraphicsScene *escena;
    QGraphicsView *vista;
    QTimer *temporizador;

    // Conexión con tus clases
    Pelota *miPelota;                         // Objeto físico
    QGraphicsEllipseItem *graficoPelota;     // Representación visual (el círculo)
};

#endif // MAINWINDOW_H
