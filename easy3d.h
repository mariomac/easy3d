#ifndef EASY3D_H
#define EASY3D_H

#include<SDL2/SDL_opengl.h>
#include<SDL2/SDL.h>
#include<stdio.h>
#include<stdlib.h>

#define PULSADO 1
#define SUELTO 0

#define FPS 30.0

#define TECLA_ESC 1

typedef struct {
	int ancho, alto;
	SDL_Window *wnd;
	SDL_GLContext glctx;
	int teclas;
} tventana;

tventana ventana;

void abre_ventana() {
	stderr = fopen("err.txt","w");
	fprintf(stderr, "ATENCION: este archivo solo esta para pillar algunos mensajes del sistema. Ignoralos si no sabes que quieren decir\n");
	if(SDL_Init(SDL_INIT_TIMER|SDL_INIT_VIDEO) == -1) {
		fprintf(stderr,"Ocurrio un error al iniciar programa: %s\n",SDL_GetError());
		exit(-1);
	}
	ventana.teclas = 0;
	ventana.ancho = 640;
	ventana.alto = 480;
	ventana.wnd = SDL_CreateWindow("Easy3D window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
							 ventana.ancho, ventana.alto, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

	//Create context
	if( !ventana.wnd ) {
		printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
		exit(-1);
	}
	ventana.glctx = SDL_GL_CreateContext( ventana.wnd );
	if( ventana.glctx == NULL )
	{
		printf( "OpenGL context could not be created! SDL Error: %s\n", SDL_GetError() );
		exit(-1);
	}
	else
	{
		//Use Vsync
		if( SDL_GL_SetSwapInterval( 1 ) < 0 )
		{
			fprintf(stderr, "Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError() );
		}

		//Initialize OpenGL
		
	}
 
}

void cierra_ventana() {
	SDL_DestroyWindow(ventana.wnd);
	SDL_Quit();
}

void nuevo_fotograma() {
	SDL_Delay((double)(1000.0/FPS));
	SDL_Event event;
	while(SDL_PollEvent(&event)) {
		if(event.type == SDL_KEYDOWN) {
			switch(event.key.keysym.sym) {
				case SDLK_ESCAPE:
					ventana.teclas |= TECLA_ESC;
					break;
			}
		} else if(event.type = SDL_KEYUP) {
			switch(event.key.keysym.sym) {
				case SDLK_ESCAPE:
					ventana.teclas &= ~TECLA_ESC;
					break;
			}		
		}
	}
	
}

int tecla_pulsada(int tecla) {
	return ventana.teclas & tecla;
}

#endif

