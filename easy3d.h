#ifndef EASY3D_H
#define EASY3D_H

#include<SDL2/SDL_opengl.h>
#include<SDL2/SDL.h>
#include<GL/gl.h>
#include<GL/glu.h>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#define PULSADO 1
#define SUELTO 0

#define ZNEAR 1
#define ZFAR 200
#define FOVY 60

#define FPS 60.0

#define TECLA_ESC 0b1
#define TECLA_ARRIBA 2
#define TECLA_ABAJO 4
#define TECLA_IZDA 8
#define TECLA_DCHA 16

typedef struct {
    double posX,posY,posZ;
    double anguloX;
} tcamara;

typedef struct {
    tcamara cam;
	int ancho, alto;
	SDL_Window *wnd;
	SDL_GLContext glctx;
	int teclas;
} tventana;

void cubo(double lado, double cx, double cy, double cz);

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

	glEnable(GL_DEPTH_TEST);
    glViewport(0,0, ventana.ancho, ventana.alto);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(FOVY, (double)ventana.ancho/(double)ventana.alto, 0.1, ZFAR);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CW);
    glCullFace(GL_BACK);

	//glOrtho(-4, 4, -0.5, 4, 0.1, 1000.0);
	glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void cierra_ventana() {
	SDL_DestroyWindow(ventana.wnd);
	SDL_Quit();
}

void nuevo_fotograma() {
	SDL_Delay((double)(1000.0/FPS));
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

	gluLookAt(ventana.cam.posX, ventana.cam.posY, ventana.cam.posZ,
           ventana.cam.posX + sin(ventana.cam.anguloX),
           ventana.cam.posY,
           ventana.cam.posZ - cos(ventana.cam.anguloX),
           0,1,0);
	// Multi-colored side - FRONT

    cubo(1, 0, 2, -3);
    cubo(2, 3, 2, 0);



    glFlush();


	SDL_GL_SwapWindow(ventana.wnd);

	SDL_Event event;
	while(SDL_PollEvent(&event)) {
		if(event.type == SDL_KEYDOWN) {
			switch(event.key.keysym.sym) {
				case SDLK_ESCAPE:
					ventana.teclas |= TECLA_ESC;
					break;
                case SDLK_UP:
                    ventana.teclas |= TECLA_ARRIBA;
                    break;
                case SDLK_DOWN:
                    ventana.teclas |= TECLA_ABAJO;
                    break;
                case SDLK_LEFT:
                    ventana.teclas |= TECLA_IZDA;
                    break;
                case SDLK_RIGHT:
                    ventana.teclas |= TECLA_DCHA;
                    break;
			}
		} else if(event.type = SDL_KEYUP) {
			switch(event.key.keysym.sym) {
				case SDLK_ESCAPE:
					ventana.teclas &= ~TECLA_ESC;
					break;
                case SDLK_UP:
                    ventana.teclas &= ~TECLA_ARRIBA;
                    break;
                case SDLK_DOWN:
                    ventana.teclas &= ~TECLA_ABAJO;
                    break;
                case SDLK_LEFT:
                    ventana.teclas &= ~TECLA_IZDA;
                    break;
                case SDLK_RIGHT:
                    ventana.teclas &= ~TECLA_DCHA;
                    break;
			}
		}
	}
}

void cubo(double lado, double cx, double cy, double cz) {
    double nx = cx-lado/2.0, px = cx+lado/2.0,
            ny = cy-lado/2.0, py = cy+lado/2.0,
            nz = cz-lado/2.0, pz = cz+lado/2.0;
    glPushMatrix();
    glTranslated(cx,cy,cz);
    //frente
    glBegin(GL_POLYGON);
        glColor3f(1,0,0);
        glVertex3f(nx,py,pz);
        glVertex3f(px,py,pz);
        glVertex3f(px,ny,pz);
        glVertex3f(nx,ny,pz);
    glEnd();
    //atras
    glBegin(GL_POLYGON);
        glColor3f(1,1,1);
        glVertex3f(nx,ny,nz);
        glVertex3f(px,ny,nz);
        glVertex3f(px,py,nz);
        glVertex3f(nx,py,nz);
    glEnd();
    //lado izdo
    glBegin(GL_POLYGON);
        glColor3f(0,0,1);
        glVertex3f(nx,py,nz);
        glVertex3f(nx,py,pz);
        glVertex3f(nx,ny,pz);
        glVertex3f(nx,ny,nz);
    glEnd();
    //lado dcho
    glBegin(GL_POLYGON);
        glColor3f(1,0,1);
        glVertex3f(px,ny,nz);
        glVertex3f(px,ny,pz);
        glVertex3f(px,py,pz);
        glVertex3f(px,py,nz);
    glEnd();
    glPopMatrix();
}

void actualiza_camara(tcamara cam) {
    ventana.cam = cam;
}

int tecla_pulsada(int tecla) {
	return ventana.teclas & tecla;
}

#endif

