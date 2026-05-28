#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "excepciones.h"
#include <QMessageBox>

// ======================================================================
// CONSTRUCTOR
// ======================================================================
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 1. Configurar el tamaño de la ventana principal
    this->resize(800, 600);

    // 2. Crear la escena (El "mapa" interno de 800x600 píxeles)
    escena = new QGraphicsScene(0, 0, 800, 600, this);

    // 3. Configurar la vista (La "cámara" que muestra la escena)
    vista = new QGraphicsView(escena, this);
    vista->setGeometry(0, 0, 800, 600);
    vista->setBackgroundBrush(Qt::darkGray); // Un tono gris de fondo

    // ==================================================================
    // DIBUJO DEL ESCENARIO (BLOCKOUT)
    // ==================================================================
    // QGraphicsRectItem recibe (x, y, ancho, alto)

    // PISO (Abajo, de lado a lado, 20px de grosor)
    piso = escena->addRect(0, 580, 800, 20, QPen(Qt::NoPen), QBrush(Qt::black));

    // TECHO (Arriba, de lado a lado, 20px de grosor)
    techo = escena->addRect(0, 0, 800, 20, QPen(Qt::NoPen), QBrush(Qt::black));

    // PARED IZQUIERDA (De arriba a abajo, 20px de grosor)
    paredIzquierda = escena->addRect(0, 0, 20, 600, QPen(Qt::NoPen), QBrush(Qt::black));

    // PARED DERECHA (De arriba a abajo, en el borde derecho, 20px de grosor)
    paredDerecha = escena->addRect(780, 0, 20, 600, QPen(Qt::NoPen), QBrush(Qt::black));

    // ==================================================================
    // CONFIGURACIÓN DE LA FÍSICA Y GRÁFICOS
    // ==================================================================

    // 4. Instanciar tu objeto físico (Nace en el centro, X=400, Y=300)
    miPelota = new Pelota(400.0f, 300.0f, 30, 30, 0.8f);

    // 5. Crear la representación gráfica en la escena (El círculo rojo)
    graficoPelota = escena->addEllipse(0, 0, 30, 30, QPen(Qt::red), QBrush(Qt::red));
    graficoPelota->setPos(miPelota->getPosX(), miPelota->getPosY());

    // 6. Configurar el temporizador (Game Loop)
    temporizador = new QTimer(this);
    connect(temporizador, &QTimer::timeout, this, &MainWindow::actualizarJuego);

    // Iniciar a 16 milisegundos por pulso (Aproximadamente 60 FPS)
    temporizador->start(16);

    // 7. Lanzar la pelota usando tu método físico aprobado
    miPelota->lanzar(80.0f, 0.785f);
}

// ======================================================================
// DESTRUCTOR
// ======================================================================
MainWindow::~MainWindow()
{
    delete ui;
    delete miPelota; // Limpiamos la memoria de nuestro objeto físico
}

// ======================================================================
// MOTOR DEL JUEGO (GAME LOOP)
// ======================================================================
void MainWindow::actualizarJuego()
{
    // Delta time fijo simulado para cada cuadro
    float dt = 0.05f;

    try {
        // 1. Ejecutamos tu lógica matemática
        miPelota->actualizar(dt);

        // 2. Sincronizamos los gráficos con la matemática
        graficoPelota->setPos(miPelota->getPosX(), miPelota->getPosY());

    }
    catch (const ExcepcionFisicaFueraDeLimites& e) {
        // Detenemos el reloj si la pelota sale de la pantalla
        temporizador->stop();
        QMessageBox::critical(this, "Fin del Trayecto", e.what());
    }
}
