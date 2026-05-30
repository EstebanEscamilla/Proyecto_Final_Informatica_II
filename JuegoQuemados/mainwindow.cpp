#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "excepciones.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Configuración de la Ventana
    this->resize(802, 602);
    escena = new QGraphicsScene(0, 0, 800, 600, this);

    vista = new QGraphicsView(escena, this);
    setCentralWidget(vista);
    vista->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    vista->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // Muros del Escenario
    techo = escena->addRect(0, 0, 800, 20, QPen(Qt::black), QBrush(Qt::black));
    piso = escena->addRect(0, 580, 800, 20, QPen(Qt::black), QBrush(Qt::black));
    paredIzquierda = escena->addRect(0, 0, 20, 600, QPen(Qt::black), QBrush(Qt::black));
    paredDerecha = escena->addRect(780, 0, 20, 600, QPen(Qt::black), QBrush(Qt::black));

    // ======================================================================
    // INSTANCIACIÓN DE OBJETOS FÍSICOS Y SUS SPRITES (ARTE REAL)
    // ======================================================================

    // 1. Crear Objeto Físico de la Pelota (50x50)
    miPelota = new Pelota(400.0f, 300.0f, 50, 50, 0.8f);

    // Cargar Sprite de la Pelota
    QPixmap imgPelota("pelota_dodgeball.png");
    imgPelota = imgPelota.scaled(50, 50, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    graficoPelota = escena->addPixmap(imgPelota);
    graficoPelota->setPos(miPelota->getPosX(), miPelota->getPosY());

    // 2. Crear Objeto Físico de Timmy (100x100)
    timmy = new Jugador(390.0f, 560.0f, 100, 100);

    // Cargar Sprite de Timmy
    QPixmap imgTimmy("timmy_quieto.png");
    imgTimmy = imgTimmy.scaled(100, 100, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    graficoTimmy = escena->addPixmap(imgTimmy);
    graficoTimmy->setPos(timmy->getPosX(), timmy->getPosY());

    // ======================================================================
    // INICIALIZACIÓN DE ESTADOS
    // ======================================================================
    teclaIzquierda = false;
    teclaDerecha = false;
    teclaArriba = false;
    teclaAbajo = false;
    pelotaEnMano = true;

    cargarNivel(1);

    // Motor de FPS
    temporizador = new QTimer(this);
    connect(temporizador, &QTimer::timeout, this, &MainWindow::actualizarJuego);
    temporizador->start(16); // ~60 FPS
}

MainWindow::~MainWindow()
{
    delete ui;
    delete miPelota;
    delete timmy;
}

// ======================================================================
// GESTOR DE NIVELES
// ======================================================================
void MainWindow::cargarNivel(int numeroNivel) {
    nivelActual = numeroNivel;

    if (nivelActual == 1) {
        timmy->setGravedadActiva(false);
        timmy->setEnSuelo(true);
        // Nace más al centro para que quepan sus 100px
        timmy->setPosicion(350.0f, 400.0f);
        vista->setBackgroundBrush(Qt::darkGray);
    }
    else if (nivelActual == 2) {
        timmy->setGravedadActiva(true);
        timmy->setEnSuelo(false);
        timmy->setPosicion(50.0f, 200.0f);
        vista->setBackgroundBrush(Qt::black);
    }
}

// ======================================================================
// CONTROLES DE TECLADO
// ======================================================================
void MainWindow::keyPressEvent(QKeyEvent *event) {
    if (!timmy || event->isAutoRepeat()) return;

    if (event->key() == Qt::Key_A) teclaIzquierda = true;
    if (event->key() == Qt::Key_D) teclaDerecha = true;

    if (event->key() == Qt::Key_W) {
        if (nivelActual == 1) {
            teclaArriba = true;
        } else if (nivelActual == 2) {
            timmy->saltar();
        }
    }
    if (event->key() == Qt::Key_S) teclaAbajo = true;

    if (event->key() == Qt::Key_Space) {
        timmy->iniciarCarga();
    }

    if (event->key() == Qt::Key_N) {
        if (nivelActual == 1) cargarNivel(2);
        else cargarNivel(1);
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent *event) {
    if (!timmy || event->isAutoRepeat()) return;

    if (event->key() == Qt::Key_A) teclaIzquierda = false;
    if (event->key() == Qt::Key_D) teclaDerecha = false;
    if (event->key() == Qt::Key_W) teclaArriba = false;
    if (event->key() == Qt::Key_S) teclaAbajo = false;

    if (event->key() == Qt::Key_Space) {
        float fuerzav0 = timmy->soltarCarga();
        pelotaEnMano = false;

        if (miPelota) {
            miPelota->lanzar(fuerzav0, 0.785f);
        }
    }
}

// ======================================================================
// BUCLE PRINCIPAL (GAME LOOP)
// ======================================================================
void MainWindow::actualizarJuego() {
    float dt = 0.05f;

    try {
        // ------------------------------------------------------------------
        // 1. MOVIMIENTO DE TIMMY (Límites ajustados a 100px)
        // ------------------------------------------------------------------
        if (nivelActual == 1) {
            if (teclaIzquierda) timmy->mover(-1, 0);
            if (teclaDerecha) timmy->mover(1, 0);
            if (teclaArriba) timmy->mover(0, -1);
            if (teclaAbajo) timmy->mover(0, 1);

            if (timmy->getPosX() < 20.0f) timmy->setPosicion(20.0f, timmy->getPosY());
            if (timmy->getPosX() > 680.0f) timmy->setPosicion(680.0f, timmy->getPosY());
            if (timmy->getPosY() < 20.0f) timmy->setPosicion(timmy->getPosX(), 20.0f);
            if (timmy->getPosY() > 480.0f) timmy->setPosicion(timmy->getPosX(), 480.0f);
        }
        else if (nivelActual == 2) {
            if (teclaIzquierda) timmy->mover(-1, 0);
            if (teclaDerecha) timmy->mover(1, 0);

            if (timmy->getPosX() < 20.0f) timmy->setPosicion(20.0f, timmy->getPosY());
            if (timmy->getPosX() > 680.0f) timmy->setPosicion(680.0f, timmy->getPosY());
        }

        // ------------------------------------------------------------------
        // 2. FÍSICA Y GRAVEDAD
        // ------------------------------------------------------------------
        timmy->aplicarFisica(dt);

        // ------------------------------------------------------------------
        // 3. PISO DE TIMMY (Ajustado a 480px)
        // ------------------------------------------------------------------
        if (nivelActual == 2) {
            if (timmy->getPosY() >= 480.0f) {
                timmy->setPosicion(timmy->getPosX(), 480.0f);
                timmy->setEnSuelo(true);
            }
        }

        // ------------------------------------------------------------------
        // 4. LÓGICA DE LA PELOTA (Límites ajustados a 50px)
        // ------------------------------------------------------------------
        if (pelotaEnMano) {
            // Posicionamos la pelota cerca del centro del sprite de 100x100 de Timmy
            miPelota->setPosicion(timmy->getPosX() + 50.0f, timmy->getPosY() + 25.0f);
        } else {
            miPelota->actualizar(dt);

            float px = miPelota->getPosX();
            float py = miPelota->getPosY();

            if (py >= 530.0f) { miPelota->setPosicion(px, 529.0f); miPelota->rebotarVertical(); }
            else if (py <= 20.0f) { miPelota->setPosicion(px, 21.0f); miPelota->rebotarVertical(); }

            if (px >= 730.0f) { miPelota->setPosicion(729.0f, py); miPelota->rebotarHorizontal(); }
            else if (px <= 20.0f) { miPelota->setPosicion(21.0f, py); miPelota->rebotarHorizontal(); }
        }

        // ------------------------------------------------------------------
        // 5. RENDERIZADO VISUAL
        // ------------------------------------------------------------------
        graficoPelota->setPos(miPelota->getPosX(), miPelota->getPosY());
        graficoTimmy->setPos(timmy->getPosX(), timmy->getPosY());

    }
    catch (const ExcepcionFisicaFueraDeLimites& e) {
        temporizador->stop();
        QMessageBox::critical(this, "Fin del Trayecto", e.what());
    }
}
