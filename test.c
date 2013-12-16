#include "easy3d.h"

#define VELOCIDAD 0.25
#define VROTACION 0.11
#define ALTURA_OJOS 1.8


void main() {

	tcamara cam = {50, 50, ALTURA_OJOS, 3.1416};
	abre_ventana();
	genera_mapa(100,100,10);

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
