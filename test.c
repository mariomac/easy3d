#include "easy3d.h"

#define VELOCIDAD 0.25
#define VROTACION 0.11
#define ALTURA_OJOS 1.8


void main() {
    tmapa map = {
        10,10,
        "      1233"
        "     12344"
        "     12233"
        "      1222"
        "       111"
        " 88       "
        " 88       "
        "      9   "
        "          "
        "          "
    };
	tcamara cam = {5, 5, ALTURA_OJOS, 0 };

	abre_ventana();
	carga_mapa(map);

	while(!tecla_pulsada(TECLA_ESC)) {
        if(tecla_pulsada(TECLA_IZDA)) {
            cam.anguloX -= VROTACION;
        } else if(tecla_pulsada(TECLA_DCHA)) {
            cam.anguloX += VROTACION;
        } else if(tecla_pulsada(TECLA_ARRIBA)) {
           cam.posX += VELOCIDAD * sin(ventana.cam.anguloX);
           cam.posY -= VELOCIDAD * cos(ventana.cam.anguloX);
        } else if(tecla_pulsada(TECLA_ABAJO)) {
           cam.posX -= VELOCIDAD * sin(ventana.cam.anguloX);
           cam.posY += VELOCIDAD * cos(ventana.cam.anguloX);
        }
        actualiza_camara(cam);
		nuevo_fotograma();
	}

	cierra_ventana();
}
