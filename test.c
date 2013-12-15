#include "easy3d.h"

#define VELOCIDAD 0.25
#define VROTACION 0.11
#define ALTURA_OJOS 1.8


void main() {
	tcamara cam = {0, ALTURA_OJOS, 0, 0 };

	abre_ventana();

	while(!tecla_pulsada(TECLA_ESC)) {
        if(tecla_pulsada(TECLA_IZDA)) {
            cam.anguloX -= VROTACION;
        } else if(tecla_pulsada(TECLA_DCHA)) {
            cam.anguloX += VROTACION;
        } else if(tecla_pulsada(TECLA_ARRIBA)) {
           cam.posX += VELOCIDAD * sin(ventana.cam.anguloX);
           cam.posZ -= VELOCIDAD * cos(ventana.cam.anguloX);
        } else if(tecla_pulsada(TECLA_ABAJO)) {
           cam.posX -= VELOCIDAD * sin(ventana.cam.anguloX);
           cam.posZ += VELOCIDAD * cos(ventana.cam.anguloX);
        }
        actualiza_camara(cam);
		nuevo_fotograma();
	}

	cierra_ventana();
}
