#include "easy3d.h"

void main() {
	abre_ventana();
	
	while(!tecla_pulsada(TECLA_ESC)) {
		nuevo_fotograma();
	}
	
	cierra_ventana();
}
