#include "easy3d.h"

#define VELOCIDAD 0.05
#define VROTACION 0.11

void main() {
    double siguienteX, siguienteY;
    tcamara cam;
    abre_ventana();
    tmapa mapa = {10, 10,
        " ######## "
        "#        #"
        "#  ####  #"
        "# ##    # "
        "#p#  ###  "
        "# # ####  "
        "# #     # "
        "#  ###   #"
        "#        #"
        " ######## "};
    cam = genera_mapa(mapa);

    siguienteX = cam.posX;
    siguienteY = cam.posY;
    while (!tecla_pulsada(TECLA_ESC)) {
        if (tecla_pulsada(TECLA_IZDA)) {
            cam.anguloX -= VROTACION;
        }
        if (tecla_pulsada(TECLA_DCHA)) {
            cam.anguloX += VROTACION;
        }
        if (tecla_pulsada(TECLA_ARRIBA)) {
            siguienteX = cam.posX + VELOCIDAD * sin(ventana.cam.anguloX);
            siguienteY = cam.posY - VELOCIDAD * cos(ventana.cam.anguloX);
        }
        if (tecla_pulsada(TECLA_ABAJO)) {
            siguienteX = cam.posX - VELOCIDAD * sin(ventana.cam.anguloX);
            siguienteY = cam.posY + VELOCIDAD * cos(ventana.cam.anguloX);
        }
        if (que_hay_aqui(mapa, siguienteX, siguienteY) != PARED) {
            cam.posX = siguienteX;
            cam.posY = siguienteY;
        }
        actualiza_camara(cam);
        nuevo_fotograma();
    }

    cierra_ventana();
}
