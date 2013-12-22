#include "easy3d.h"
#include "consola.h"

#define VELOCIDAD 0.05
#define VROTACION 0.11
#define ALTURA_OJOS 0.5

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
    cam = lee_mapa(mapa);
    cam.posZ = ALTURA_OJOS;
    
    int x = 0;
    siguienteX = cam.posX;
    siguienteY = cam.posY;
    short color = 0;
    
    printf("*** Bienvenido ***\n");
   
    while (!tecla_pulsada(TECLA_ESC)) {
        if (tecla_pulsada(TECLA_IZDA)) {
            cam.anguloX -= VROTACION;
        }
        if (tecla_pulsada(TECLA_DCHA)) {
            cam.anguloX += VROTACION;
        }
        if (tecla_pulsada(TECLA_ARRIBA)) {
            siguienteX = cam.posX + VELOCIDAD * sin(cam.anguloX);
            siguienteY = cam.posY - VELOCIDAD * cos(cam.anguloX);
        }
        if (tecla_pulsada(TECLA_ABAJO)) {
            siguienteX = cam.posX - VELOCIDAD * sin(cam.anguloX);
            siguienteY = cam.posY + VELOCIDAD * cos(cam.anguloX);
        }
        if (que_hay_aqui(mapa, siguienteX, siguienteY) != PARED) {
            cam.posX = siguienteX;
            cam.posY = siguienteY;
        } else {
            printf("no puedes pasar\n");
        }
        muestra_fotograma(cam);
    }

    cierra_ventana();
}
