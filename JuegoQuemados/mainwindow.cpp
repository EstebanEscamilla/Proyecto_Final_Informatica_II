#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "excepciones.h"
#include <QMessageBox>
#include <QApplication> // Para poder cerrar el juego en el Game Over

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 1. CONFIGURACIÓN DE LA VENTANA
    this->resize(802, 602);
    escena = new QGraphicsScene(0, 0, 800, 600, this);
    vista = new QGraphicsView(escena, this);
    setCentralWidget(vista);
    vista->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    vista->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // 2. MUROS DEL ESCENARIO
    techo = escena->addRect(0, 0, 800, 20, QPen(Qt::black), QBrush(Qt::black));
    piso = escena->addRect(0, 580, 800, 20, QPen(Qt::black), QBrush(Qt::black));
    paredIzquierda = escena->addRect(0, 0, 20, 600, QPen(Qt::black), QBrush(Qt::black));
    paredDerecha = escena->addRect(780, 0, 20, 600, QPen(Qt::black), QBrush(Qt::black));

    // 3. FONDOS MÓVILES Y FILTRO ÓPTICO
    QPixmap imgFondo("cinta_fondo.png");
    imgFondo = imgFondo.scaled(800, 600, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

    fondoCinta1 = escena->addPixmap(imgFondo);
    fondoCinta2 = escena->addPixmap(imgFondo);
    fondoCinta1->setPos(0, 0);
    fondoCinta2->setPos(0, -600);
    fondoCinta1->setZValue(-2);
    fondoCinta2->setZValue(-2);
    velCinta = 50.0f;

    filtroOscuro = escena->addRect(0, 0, 800, 600, Qt::NoPen, QBrush(QColor(0, 0, 0, 150)));
    filtroOscuro->setZValue(-1);

    // 4. INSTANCIACIÓN DE OBJETOS
    miPelota = new Pelota(400.0f, 300.0f, 50, 50, 0.8f);
    QPixmap imgPelota("pelota_dodgeball.png");
    imgPelota = imgPelota.scaled(50, 50, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    graficoPelota = escena->addPixmap(imgPelota);

    timmy = new Jugador(390.0f, 560.0f, 100, 100);
    QPixmap imgTimmy("timmy_quieto.png");
    imgTimmy = imgTimmy.scaled(100, 100, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    graficoTimmy = escena->addPixmap(imgTimmy);

    osoTest = new OsoPeluche(350.0f, 50.0f, 100, 100);
    QPixmap imgOso("oso_peluche.png");
    imgOso = imgOso.scaled(100, 100, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    graficoOsoTest = escena->addPixmap(imgOso);

    gancho = new Pendulo(400.0f, 150.0f, 60, 60, 250.0f, 2.5f);
    QPixmap imgGancho("gancho.png");
    imgGancho = imgGancho.scaled(60, 60, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    graficoGancho = escena->addPixmap(imgGancho);

    // ======================================================================
    // NUEVO: CONFIGURACIÓN DEL HUD (TEXTOS EN PANTALLA)
    // ======================================================================
    textoVidas = new QGraphicsTextItem();
    textoVidas->setFont(QFont("Arial", 16, QFont::Bold));
    textoVidas->setDefaultTextColor(Qt::red);
    textoVidas->setPos(30, 30);
    textoVidas->setZValue(10); // Siempre por encima de todo
    escena->addItem(textoVidas);

    textoTiempo = new QGraphicsTextItem();
    textoTiempo->setFont(QFont("Arial", 16, QFont::Bold));
    textoTiempo->setDefaultTextColor(Qt::white);
    textoTiempo->setPos(600, 30);
    textoTiempo->setZValue(10);
    escena->addItem(textoTiempo);

    // 5. INICIALIZACIÓN
    teclaIzquierda = false;
    teclaDerecha = false;
    teclaArriba = false;
    teclaAbajo = false;
    pelotaEnMano = true;
    framesSobrevividos = 0;

    cargarNivel(1);

    temporizador = new QTimer(this);
    connect(temporizador, &QTimer::timeout, this, &MainWindow::actualizarJuego);
    temporizador->start(16); // 60 FPS
}

MainWindow::~MainWindow()
{
    limpiarBotones();
    delete ui;
    delete miPelota;
    delete timmy;
    delete osoTest;
    if (gancho) delete gancho;
}

// ======================================================================
// GESTOR DE NIVELES
// ======================================================================
void MainWindow::cargarNivel(int numeroNivel) {
    nivelActual = numeroNivel;
    limpiarBotones();

    if (nivelActual == 1) {
        timmy->setGravedadActiva(false);
        timmy->setEnSuelo(true);
        timmy->setPosicion(350.0f, 400.0f);
        vista->setBackgroundBrush(Qt::darkGray);

        if (fondoCinta1) fondoCinta1->show();
        if (fondoCinta2) fondoCinta2->show();
        if (filtroOscuro) filtroOscuro->show();
        if (graficoOsoTest) graficoOsoTest->show();
        if (graficoGancho) graficoGancho->hide();

        framesSobrevividos = 0; // Reiniciamos el tiempo al iniciar el Nivel 1
    }
    else if (nivelActual == 2) {
        timmy->setGravedadActiva(true);
        timmy->setEnSuelo(false);
        timmy->setPosicion(50.0f, 200.0f);
        vista->setBackgroundBrush(Qt::black);

        if (fondoCinta1) fondoCinta1->hide();
        if (fondoCinta2) fondoCinta2->hide();
        if (filtroOscuro) filtroOscuro->hide();
        if (graficoOsoTest) graficoOsoTest->hide();
        if (graficoGancho) graficoGancho->show();

        textoTiempo->setPlainText(QString("¡JEFE FINAL!")); // Cambiamos el texto
    }
}

void MainWindow::limpiarBotones() {
    for (auto grafico : listaGraficosBotones) {
        escena->removeItem(grafico);
        delete grafico;
    }
    for (auto objetoFisico : listaBotones) {
        delete objetoFisico;
    }
    listaBotones.clear();
    listaGraficosBotones.clear();
}

// ======================================================================
// NUEVO: FUNCIÓN GAME OVER
// ======================================================================
void MainWindow::gameOver() {
    temporizador->stop(); // Detenemos el Game Loop
    QMessageBox::critical(this, "Game Over", "¡Timmy ha perdido todas sus vidas!\nLa fábrica lo ha destruido.");
    QApplication::quit(); // Cierra la aplicación
}

// Controles de Teclado
void MainWindow::keyPressEvent(QKeyEvent *event) {
    if (!timmy || event->isAutoRepeat()) return;
    if (event->key() == Qt::Key_A) teclaIzquierda = true;
    if (event->key() == Qt::Key_D) teclaDerecha = true;
    if (event->key() == Qt::Key_W) {
        if (nivelActual == 1) teclaArriba = true;
        else if (nivelActual == 2) timmy->saltar();
    }
    if (event->key() == Qt::Key_S) teclaAbajo = true;
    if (event->key() == Qt::Key_Space) timmy->iniciarCarga();
    // Quitamos la tecla 'N' para evitar saltar el nivel haciendo trampa
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
        if (miPelota) miPelota->lanzar(fuerzav0, 0.785f);
    }
}

// ======================================================================
// BUCLE PRINCIPAL (GAME LOOP)
// ======================================================================
void MainWindow::actualizarJuego() {
    float dt = 0.05f;

    try {
        // ==================================================================
        // NUEVO: ACTUALIZACIÓN DEL HUD Y CONDICIONES DE VICTORIA/DERROTA
        // ==================================================================
        // Actualizamos el texto de vidas rojas en la esquina
        textoVidas->setPlainText(QString("Vidas: %1").arg(timmy->getVidas()));

        // Comprobamos si perdimos
        if (timmy->getVidas() <= 0) {
            gameOver();
            return; // Salimos de la función inmediatamente para que nada más se actualice
        }

        // Lógica de supervivencia del Nivel 1
        if (nivelActual == 1) {
            framesSobrevividos++;
            // Como el timer corre a 60 FPS, 60 iteraciones son 1 segundo real
            int segundos = framesSobrevividos / 60;
            textoTiempo->setPlainText(QString("Tiempo: %1 / 30s").arg(segundos));

            // Si sobrevivimos 30 segundos, ganamos el nivel
            if (segundos >= 30) {
                cargarNivel(2);
            }
        }

        // ==================================================================
        // ANIMACIÓN DEL FONDO (CINTA TRANSPORTADORA)
        // ==================================================================
        if (nivelActual == 1 && fondoCinta1 && fondoCinta2) {
            float nuevaY1 = fondoCinta1->y() + (velCinta * dt);
            float nuevaY2 = fondoCinta2->y() + (velCinta * dt);
            if (nuevaY1 >= 600.0f) nuevaY1 = -600.0f;
            if (nuevaY2 >= 600.0f) nuevaY2 = -600.0f;
            fondoCinta1->setY(nuevaY1);
            fondoCinta2->setY(nuevaY2);
        }

        // Movimiento de Timmy
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

        // Física de Timmy
        timmy->aplicarFisica(dt);
        if (nivelActual == 2) {
            if (timmy->getPosY() >= 480.0f) {
                timmy->setPosicion(timmy->getPosX(), 480.0f);
                timmy->setEnSuelo(true);
            }
        }

        // Física del Gancho
        if (nivelActual == 2 && gancho) {
            gancho->aplicarFisica(dt);
            float gx = gancho->getPosX();
            float gy = gancho->getPosY();
            float tx = timmy->getPosX();
            float ty = timmy->getPosY();
            bool colisionX = (gx + 60.0f >= tx) && (tx + 100.0f >= gx);
            bool colisionY = (gy + 60.0f >= ty) && (ty + 100.0f >= gy);
            if (colisionX && colisionY) {
                timmy->reducirVida();
                timmy->setPosicion(50.0f, 200.0f);
            }
        }

        // Inteligencia del Oso
        if (nivelActual == 1 && osoTest) {
            BotonAfilado* nuevoBoton = osoTest->disparar(dt, 0.0f, 220.0f);
            if (nuevoBoton != nullptr) {
                listaBotones.push_back(nuevoBoton);
                QPixmap imgBoton("boton_afilado.png");
                imgBoton = imgBoton.scaled(40, 40, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
                QGraphicsPixmapItem* grafBoton = escena->addPixmap(imgBoton);
                grafBoton->setPos(nuevoBoton->getPosX(), nuevoBoton->getPosY());
                listaGraficosBotones.push_back(grafBoton);
            }
        }

        // Colisiones Botones
        for (size_t i = 0; i < listaBotones.size(); ) {
            listaBotones[i]->aplicarFisica(dt);
            listaGraficosBotones[i]->setPos(listaBotones[i]->getPosX(), listaBotones[i]->getPosY());

            float bx = listaBotones[i]->getPosX();
            float by = listaBotones[i]->getPosY();
            float tx = timmy->getPosX();
            float ty = timmy->getPosY();

            bool colisionX = (bx + 40.0f >= tx) && (tx + 100.0f >= bx);
            bool colisionY = (by + 40.0f >= ty) && (ty + 100.0f >= by);

            if (colisionX && colisionY) {
                timmy->reducirVida();
                escena->removeItem(listaGraficosBotones[i]);
                delete listaGraficosBotones[i];
                delete listaBotones[i];
                listaBotones.erase(listaBotones.begin() + i);
                listaGraficosBotones.erase(listaGraficosBotones.begin() + i);
                continue;
            }
            if (by > 580.0f) {
                escena->removeItem(listaGraficosBotones[i]);
                delete listaGraficosBotones[i];
                delete listaBotones[i];
                listaBotones.erase(listaBotones.begin() + i);
                listaGraficosBotones.erase(listaGraficosBotones.begin() + i);
            } else {
                i++;
            }
        }

        // Lógica de la Pelota
        if (pelotaEnMano) {
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

        // Renderizado
        graficoPelota->setPos(miPelota->getPosX(), miPelota->getPosY());
        graficoTimmy->setPos(timmy->getPosX(), timmy->getPosY());
        if (nivelActual == 2 && graficoGancho && gancho) {
            graficoGancho->setPos(gancho->getPosX(), gancho->getPosY());
        }

    }
    catch (const ExcepcionFisicaFueraDeLimites& e) {
        temporizador->stop();
        QMessageBox::critical(this, "Fin del Trayecto", e.what());
    }
}
