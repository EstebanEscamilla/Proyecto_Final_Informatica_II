#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "excepciones.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    // 1. Configurar el tamaño de la ventana
    this->resize(800, 600);

    // 2. Crear la escena (un lienzo de 800x600 píxeles)
    escena = new QGraphicsScene(0, 0, 800, 600, this);

    // 3. Configurar la vista (cámara) para mostrar la escena
    vista = new QGraphicsView(escena, this);
    vista->setGeometry(0, 0, 800, 600);
    vista->setBackgroundBrush(Qt::black); // Fondo negro para pruebas

    // 4. Instanciar tu objeto físico (X=50, Y=500, ancho=20, alto=20, elasticidad=0.8)
    miPelota = new Pelota(50.0f, 500.0f, 20, 20, 0.8f);

    // 5. Crear la representación gráfica en la escena (un círculo rojo)
    // Se crea en (0,0) localmente, luego la moveremos con la posición física
    graficoPelota = escena->addEllipse(0, 0, 20, 20, QPen(Qt::red), QBrush(Qt::red));
    graficoPelota->setPos(miPelota->getPosX(), miPelota->getPosY());

    // 6. Configurar el temporizador (Game Loop)
    temporizador = new QTimer(this);
    connect(temporizador, &QTimer::timeout, this, &MainWindow::actualizarJuego);

    // Iniciar a 16 milisegundos por pulso (Aproximadamente 60 FPS)
    temporizador->start(16);

    // 7. Lanzar la pelota usando tu método físico aprobado
    // Fuerza = 80.0f, Angulo = 45 grados (0.785 radianes)
    miPelota->lanzar(80.0f, 0.785f);
}

MainWindow::~MainWindow() {
    delete ui;
    delete miPelota; // Limpieza de memoria
}

void MainWindow::actualizarJuego() {
    // Delta time fijo simulado para cada cuadro
    float dt = 0.05f;

    try {
        // Ejecutamos tu lógica física aprobada
        miPelota->actualizar(dt);

        // Sincronizamos los gráficos con la matemática física
        // Trasladamos las coordenadas calculadas al círculo visual en pantalla
        graficoPelota->setPos(miPelota->getPosX(), miPelota->getPosY());

    }
    catch (const ExcepcionFisicaFueraDeLimites& e) {
        // Si tu excepción física salta, detenemos el juego limpiamente
        temporizador->stop();

        // Qt nos permite mostrar el error de tu excepción en una ventana flotante real
        QMessageBox::critical(this, "Fin del Trayecto", e.what());
    }
}
