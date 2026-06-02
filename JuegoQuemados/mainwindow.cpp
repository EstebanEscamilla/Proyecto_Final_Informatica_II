#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "excepciones.h"
#include <QMessageBox>
#include <QApplication>

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
    QPixmap imgFondo("cinta_fondo.jpeg");
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

    // 4. FONDO CINTA NIVEL 2
    QPixmap imgFondoNivel2("fondo_fabrica.jpeg");
    imgFondoNivel2 = imgFondoNivel2.scaled(800, 600, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    fondoFijoNivel2 = escena->addPixmap(imgFondoNivel2);
    fondoFijoNivel2->setPos(0, 0);
    fondoFijoNivel2->setZValue(-3);

    // 5. INSTANCIACIÓN DE OBJETOS
    miPelota = new Pelota(400.0f, 300.0f, 50, 50, 0.8f);
    QPixmap imgPelota("pelota_dodgeball.png");
    imgPelota = imgPelota.scaled(50, 50, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    graficoPelota = escena->addPixmap(imgPelota);

    timmy = new Jugador(390.0f, 560.0f, 100, 100);
    QPixmap imgTimmy("timmy_quieto.png");
    imgTimmy = imgTimmy.scaled(100, 100, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    graficoTimmy = escena->addPixmap(imgTimmy);

    osoTest = new OsoPeluche(390.0f, 50.0f, 100, 100);
    QPixmap imgOso("oso_peluche.png");
    imgOso = imgOso.scaled(100, 100, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    graficoOsoTest = escena->addPixmap(imgOso);
    graficoOsoTest->setPos(osoTest->getPosX(), osoTest->getPosY()); // Sincronizado

    gancho = new Pendulo(400.0f, 540.0f, 70, 70, 650.0f, 0.5f);
    QPixmap imgGancho("gancho.png");
    imgGancho = imgGancho.scaled(60, 60, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    graficoGancho = escena->addPixmap(imgGancho);

    jefeFinal = new MunecoCuerda(650.0f, 380.0f, 100, 100);
    QPixmap imgJefe("muneco_cuerda.png");
    imgJefe = imgJefe.scaled(100, 100, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    graficoJefe = escena->addPixmap(imgJefe);
    graficoJefe->setPos(jefeFinal->getPosX(), jefeFinal->getPosY());

    QPixmap imgBonus("bonus_tiempo.png");
    imgBonus = imgBonus.scaled(40, 40, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    graficoBonus = escena->addPixmap(imgBonus);
    graficoBonus->hide(); // Inicia invisible

    // ======================================================================
    // CONFIGURACIÓN DEL HUD (CORAZONES Y TIEMPO)
    // ======================================================================
    QPixmap imgCorazon("corazon.png");
    imgCorazon = imgCorazon.scaled(70, 70, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

    for (int i = 0; i < 3; i++) {
        QGraphicsPixmapItem* graficoCorazon = escena->addPixmap(imgCorazon);
        graficoCorazon->setPos(30 + (i * 40), 30);
        graficoCorazon->setZValue(10);
        iconosVidas.push_back(graficoCorazon);
    }

    textoTiempo = new QGraphicsTextItem();
    textoTiempo->setFont(QFont("Arial", 16, QFont::Bold));
    textoTiempo->setDefaultTextColor(Qt::white);
    textoTiempo->setPos(600, 30);
    textoTiempo->setZValue(10);
    escena->addItem(textoTiempo);

    // 5. INICIALIZACIÓN
    direccionOso = 1;
    teclaIzquierda = false;
    teclaDerecha = false;
    teclaArriba = false;
    teclaAbajo = false;
    pelotaEnMano = true;
    framesSobrevividos = 0;

    bonusActivo = false;
    tiempoParaBonus = 8.0f; // El primer bonus caerá a los 8 segundos

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
    if (jefeFinal) delete jefeFinal;
}

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
        if (graficoJefe) graficoJefe->hide();
        if (fondoFijoNivel2) fondoFijoNivel2->hide();

        framesSobrevividos = 0;
        pelotaEnMano = true; // Aseguramos que inicie con balón
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
        if (graficoJefe) graficoJefe->show();
        if (fondoFijoNivel2) fondoFijoNivel2->show();

        textoTiempo->setPlainText(QString("¡JEFE FINAL!"));
        pelotaEnMano = true; // Recarga para el jefe
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

void MainWindow::gameOver() {
    temporizador->stop();
    QMessageBox::critical(this, "Game Over", "¡Timmy ha perdido todas sus vidas!\nLa fábrica lo ha destruido.");
    QApplication::quit();
}

void MainWindow::victoria() {
    temporizador->stop();
    QMessageBox::information(this, "¡VICTORIA!", "¡Has derrotado al Muñeco de Cuerda y escapado de la fábrica!\n¡Felicidades!");
    QApplication::quit();
}

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
}

void MainWindow::keyReleaseEvent(QKeyEvent *event) {
    if (!timmy || event->isAutoRepeat()) return;
    if (event->key() == Qt::Key_A) teclaIzquierda = false;
    if (event->key() == Qt::Key_D) teclaDerecha = false;
    if (event->key() == Qt::Key_W) teclaArriba = false;
    if (event->key() == Qt::Key_S) teclaAbajo = false;

    if (event->key() == Qt::Key_Space) {
        float fuerzav0 = timmy->soltarCarga();

        // REGLA: Solo dispara si el balón ya regresó a la mano
        if (pelotaEnMano) {
            pelotaEnMano = false;

            // En Nivel 2 usamos el tiro parabólico con gravedad
            if (nivelActual == 2 && miPelota) {
                miPelota->lanzar(fuerzav0, 0.785f); // 45 grados
            }
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
        // 1. ACTUALIZACIÓN DEL HUD (CORAZONES Y TIEMPO)
        // ------------------------------------------------------------------
        int vidasActuales = timmy->getVidas();
        for (int i = 0; i < (int)iconosVidas.size(); i++) {
            if (i < vidasActuales) {
                iconosVidas[i]->show();
            } else {
                iconosVidas[i]->hide();
            }
        }

        if (timmy->getVidas() <= 0) {
            gameOver();
            return;
        }

        if (nivelActual == 1) {
            framesSobrevividos++;
            int segundos = framesSobrevividos / 60;
            textoTiempo->setPlainText(QString("Tiempo: %1 / 60s").arg(segundos));
            if (segundos >= 60) {
                cargarNivel(2);
            }
        }
        else if (nivelActual == 2 && jefeFinal) {
            textoTiempo->setPlainText(QString("Salud Jefe: %1 / 5").arg(jefeFinal->getSalud()));
        }

        // ------------------------------------------------------------------
        // 2. FONDOS Y MÁRGENES (NIVEL 1)
        // ------------------------------------------------------------------
        if (nivelActual == 1 && fondoCinta1 && fondoCinta2) {
            float nuevaY1 = fondoCinta1->y() + (velCinta * dt);
            float nuevaY2 = fondoCinta2->y() + (velCinta * dt);
            if (nuevaY1 >= 600.0f) nuevaY1 = -600.0f;
            if (nuevaY2 >= 600.0f) nuevaY2 = -600.0f;
            fondoCinta1->setY(nuevaY1);
            fondoCinta2->setY(nuevaY2);
        }

        if (nivelActual == 1) {
            if (teclaIzquierda) timmy->mover(-1, 0);
            if (teclaDerecha) timmy->mover(1, 0);
            if (teclaArriba) timmy->mover(0, -1);
            if (teclaAbajo) timmy->mover(0, 1);

            // Márgenes del Nivel 1 (Ancho de cinta)
            if (timmy->getPosX() < 200.0f) timmy->setPosicion(200.0f, timmy->getPosY());
            if (timmy->getPosX() > 500.0f) timmy->setPosicion(500.0f, timmy->getPosY());
            if (timmy->getPosY() < 20.0f) timmy->setPosicion(timmy->getPosX(), 20.0f);
            if (timmy->getPosY() > 480.0f) timmy->setPosicion(timmy->getPosX(), 480.0f);
        }
        else if (nivelActual == 2) {
            if (teclaIzquierda) timmy->mover(-1, 0);
            if (teclaDerecha) timmy->mover(1, 0);

            if (timmy->getPosX() < 20.0f) timmy->setPosicion(20.0f, timmy->getPosY());
            if (timmy->getPosX() > 680.0f) timmy->setPosicion(680.0f, timmy->getPosY());
        }

        // ==================================================================
        // LÓGICA DEL ITEM BONUS (SOLO NIVEL 1)
        // ==================================================================
        if (nivelActual == 1) {
            if (!bonusActivo) {
                tiempoParaBonus -= dt;
                if (tiempoParaBonus <= 0.0f) {
                    bonusActivo = true;
                    // Aparece en una coordenada X aleatoria dentro de la cinta (200 a 460)
                    float randomX = 200.0f + static_cast<float>(rand() % 260);
                    graficoBonus->setPos(randomX, -50.0f);
                    graficoBonus->show();
                }
            } else {
                // El bonus baja a la misma velocidad de la cinta transportadora
                float currentY = graficoBonus->y();
                graficoBonus->setY(currentY + (velCinta * dt));

                // Colisión Timmy vs Bonus
                float boxX = graficoBonus->x();
                float boxY = graficoBonus->y();
                float tx = timmy->getPosX();
                float ty = timmy->getPosY();

                bool hitX = (boxX + 40.0f >= tx) && (tx + 100.0f >= boxX);
                bool hitY = (boxY + 40.0f >= ty) && (ty + 100.0f >= boxY);

                if (hitX && hitY) {
                    // ¡Premio! regala 10 segundos enteros en el cronómetro
                    framesSobrevividos += 600;

                    bonusActivo = false;
                    graficoBonus->hide();
                    tiempoParaBonus = 12.0f; // Volverá a caer otro en 12 segundos
                }
                else if (boxY > 600.0f) {
                    // Si se te escapó por abajo
                    bonusActivo = false;
                    graficoBonus->hide();
                    tiempoParaBonus = 12.0f;
                }
            }
        }

        // ------------------------------------------------------------------
        // 3. FÍSICAS BASE (TIMMY Y CAJA EMBRUJADA)
        // ------------------------------------------------------------------
        timmy->aplicarFisica(dt);
        if (nivelActual == 2) {
            if (timmy->getPosY() >= 480.0f) {
                timmy->setPosicion(timmy->getPosX(), 480.0f);
                timmy->setEnSuelo(true);
            }
        }

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

        // ------------------------------------------------------------------
        // 4. ENEMIGOS (JEFE Y OSO)
        // ------------------------------------------------------------------
        if (nivelActual == 2 && jefeFinal) {
            jefeFinal->aplicarFisica(dt);
            jefeFinal->pensar(dt, timmy->getPosX(), timmy->getPosY());

            float jx = jefeFinal->getPosX();
            float jy = jefeFinal->getPosY();
            float tx = timmy->getPosX();
            float ty = timmy->getPosY();
            float px = miPelota->getPosX();
            float py = miPelota->getPosY();

            bool colisionTimmyJefeX = (jx + 100.0f >= tx) && (tx + 100.0f >= jx);
            bool colisionTimmyJefeY = (jy + 100.0f >= ty) && (ty + 100.0f >= jy);
            if (colisionTimmyJefeX && colisionTimmyJefeY && jefeFinal->getEstado() != ENFRIAMIENTO) {
                timmy->reducirVida();
                timmy->setPosicion(50.0f, 200.0f);
            }

            if (!pelotaEnMano) {
                bool colisionPelotaJefeX = (jx + 100.0f >= px) && (px + 50.0f >= jx);
                bool colisionPelotaJefeY = (jy + 100.0f >= py) && (py + 50.0f >= jy);
                if (colisionPelotaJefeX && colisionPelotaJefeY && jefeFinal->getEstado() != ENFRIAMIENTO) {
                    jefeFinal->recibirDano();
                    miPelota->rebotarHorizontal();
                }
            }

            if (jefeFinal->getSalud() <= 0) {
                victoria();
                return;
            }
        }

        if (nivelActual == 1 && osoTest) {
            // Patrullaje
            float velPatrullaje = 100.0f;
            float nuevaPosX = osoTest->getPosX() + (direccionOso * velPatrullaje * dt);
            osoTest->setPosicion(nuevaPosX, osoTest->getPosY());

            if (osoTest->getPosX() >= 550.0f) direccionOso = -1;
            if (osoTest->getPosX() <= 150.0f) direccionOso = 1;

            // Disparo
            BotonAfilado* nuevoBoton = osoTest->disparar(dt, 0.0f, 220.0f);
            if (nuevoBoton != nullptr) {
                listaBotones.push_back(nuevoBoton);
                QPixmap imgBoton("boton_afilado.png");
                imgBoton = imgBoton.scaled(60, 60, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
                QGraphicsPixmapItem* grafBoton = escena->addPixmap(imgBoton);
                grafBoton->setPos(nuevoBoton->getPosX(), nuevoBoton->getPosY());
                listaGraficosBotones.push_back(grafBoton);
            }
        }

        // COLISIONES BOTONES AFILADOS
        for (size_t i = 0; i < listaBotones.size(); ) {
            listaBotones[i]->aplicarFisica(dt);
            listaGraficosBotones[i]->setPos(listaBotones[i]->getPosX(), listaBotones[i]->getPosY());

            float bx = listaBotones[i]->getPosX();
            float by = listaBotones[i]->getPosY();
            float tx = timmy->getPosX();
            float ty = timmy->getPosY();

            // Colisión Pelota vs Botón (Mecánica de Escudo)
            if (!pelotaEnMano) {
                float px = miPelota->getPosX();
                float py = miPelota->getPosY();

                bool choquePelotaX = (bx + 60.0f >= px) && (px + 50.0f >= bx);
                bool choquePelotaY = (by + 60.0f >= py) && (py + 50.0f >= by);

                if (choquePelotaX && choquePelotaY) {
                    pelotaEnMano = true; // Tu balón rebota de vuelta a ti al instante

                    // Destruimos el botón enemigo
                    escena->removeItem(listaGraficosBotones[i]);
                    delete listaGraficosBotones[i];
                    delete listaBotones[i];
                    listaBotones.erase(listaBotones.begin() + i);
                    listaGraficosBotones.erase(listaGraficosBotones.begin() + i);
                    continue; // Saltamos a comprobar el siguiente botón
                }
            }

            // 2. Colisión Botón vs Timmy
            bool colisionX = (bx + 60.0f >= tx) && (tx + 100.0f >= bx);
            bool colisionY = (by + 60.0f >= ty) && (ty + 100.0f >= by);

            if (colisionX && colisionY) {
                timmy->reducirVida();
                escena->removeItem(listaGraficosBotones[i]);
                delete listaGraficosBotones[i];
                delete listaBotones[i];
                listaBotones.erase(listaBotones.begin() + i);
                listaGraficosBotones.erase(listaGraficosBotones.begin() + i);
                continue;
            }

            // 3. Destruir si sale de la pantalla
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

        // ------------------------------------------------------------------
        // 5. LÓGICA DE LA PELOTA Y RECARGAS
        // ------------------------------------------------------------------
        if (pelotaEnMano) {
            miPelota->setPosicion(timmy->getPosX() + 25.0f, timmy->getPosY() - 20.0f);
        } else {
            if (nivelActual == 1) {
                // MRU Línea Recta
                float nuevaY = miPelota->getPosY() - (400.0f * dt);
                miPelota->setPosicion(miPelota->getPosX(), nuevaY);

                float px = miPelota->getPosX();
                float py = miPelota->getPosY();

                // Falla (Toca el Techo)
                if (py <= 20.0f) {
                    pelotaEnMano = true;
                }

                // Acierto (Golpea al Oso)
                if (osoTest) {
                    float ox = osoTest->getPosX();
                    float oy = osoTest->getPosY();
                    bool colisionOsoX = (ox + 100.0f >= px) && (px + 50.0f >= ox);
                    bool colisionOsoY = (oy + 100.0f >= py) && (py + 50.0f >= oy);

                    if (colisionOsoX && colisionOsoY) {
                        pelotaEnMano = true;
                        framesSobrevividos += 300; // Premio: +5 Segundos
                    }
                }
            }
            else if (nivelActual == 2) {
                // Física Parabólica
                miPelota->actualizar(dt);
                float px = miPelota->getPosX();
                float py = miPelota->getPosY();
                if (py >= 530.0f) { miPelota->setPosicion(px, 529.0f); miPelota->rebotarVertical(); }
                else if (py <= 20.0f) { miPelota->setPosicion(px, 21.0f); miPelota->rebotarVertical(); }
                if (px >= 730.0f) { miPelota->setPosicion(729.0f, py); miPelota->rebotarHorizontal(); }
                else if (px <= 20.0f) { miPelota->setPosicion(21.0f, py); miPelota->rebotarHorizontal(); }
            }
        }

        // ------------------------------------------------------------------
        // 6. RENDERIZADO VISUAL
        // ------------------------------------------------------------------
        graficoPelota->setPos(miPelota->getPosX(), miPelota->getPosY());
        graficoTimmy->setPos(timmy->getPosX(), timmy->getPosY());

        if (nivelActual == 1 && graficoOsoTest && osoTest) {
            graficoOsoTest->setPos(osoTest->getPosX(), osoTest->getPosY());
        }
        if (nivelActual == 2) {
            if (graficoGancho && gancho) graficoGancho->setPos(gancho->getPosX(), gancho->getPosY());
            if (graficoJefe && jefeFinal) graficoJefe->setPos(jefeFinal->getPosX(), jefeFinal->getPosY());
        }
    }
    catch (const ExcepcionFisicaFueraDeLimites& e) {
        temporizador->stop();
        QMessageBox::critical(this, "Fin del Trayecto", e.what());
    }
}
