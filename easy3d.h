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

#define TAM_TILE 1.0

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

typedef struct {
    int xtiles, ytiles;
    char *tiles;
} tmapa;

typedef struct {
    GLuint mlist;
    tmapa mapa;
} tescenario;

void cubo(double lado, double cx, double cy, double cz);

tventana ventana;
tescenario esc = { -1 };

void abre_ventana() {
	stderr = fopen("err.txt","w");
	fprintf(stderr, "ATENCION: este archivo solo esta para pillar algunos mensajes del sistema."
         " Ignoralos si no sabes que quieren decir\n");
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
    gluPerspective(FOVY, (double)-ventana.ancho/(double)ventana.alto, 0.1, ZFAR);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CW);
    glCullFace(GL_BACK);

    // Enable lighting
    glEnable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);


	//glOrtho(-4, 4, -0.5, 4, 0.1, 1000.0);
	glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void cierra_ventana() {
	SDL_DestroyWindow(ventana.wnd);
	SDL_Quit();
	free(esc.mapa.tiles);
}

void nuevo_fotograma() {
	SDL_Delay((double)(1000.0/FPS));
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

	gluLookAt(ventana.cam.posX, ventana.cam.posY, ventana.cam.posZ,
           ventana.cam.posX + sin(ventana.cam.anguloX),
           ventana.cam.posY - cos(ventana.cam.anguloX),
           ventana.cam.posZ,
           0,0,1);
	// Multi-colored side - FRONT

    GLfloat lightcol[] = {0.3,0.3,0.3, 0};
    GLfloat ambient[] = {0.1,0.1,0.1, 0};

glLightfv(GL_LIGHT0, GL_AMBIENT, ambient );
glLightfv(GL_LIGHT1, GL_AMBIENT, ambient );
glLightfv(GL_LIGHT0, GL_SPECULAR, lightcol );
glLightfv(GL_LIGHT1, GL_SPECULAR, lightcol );
//glLightfv(GL_LIGHTn, GL_SPECULAR, lightcol );
    GLfloat lightpos[] = {20, 10, 10, 0};
    glLightfv(GL_LIGHT0, GL_POSITION, lightpos);
    GLfloat lightpos2[] = {-20, 10, 10, 0};
    glLightfv(GL_LIGHT1, GL_POSITION, lightpos2);

    glCallList(esc.mlist);

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
    double n = -lado/2.0, p = lado/2.0;
    glPushMatrix();
    glTranslated(cx,cy,cz);
    //frente
    glColor3f(1,0,0);

    GLUquadric *c = gluNewQuadric();
    gluSphere(c, lado/2, 3, 3);
    gluDeleteQuadric(c);

    glPopMatrix();
}

void actualiza_camara(tcamara cam) {
    ventana.cam = cam;
}

int tecla_pulsada(int tecla) {
	return ventana.teclas & tecla;
}

double get_altura_terreno(double x, double y) {
    int xx = (int) x;
    int yy = (int) y;
    if(xx >= 0 && xx < esc.mapa.xtiles && yy >= 0 && yy < esc.mapa.ytiles) {
        return esc.mapa.tiles[yy * esc.mapa.xtiles + xx];
    } else {
        return 0;
    }
}

void carga_mapa(tmapa m) {
    int x,y; char c;
    double t,o;
    esc.mapa.xtiles = m.xtiles;
    esc.mapa.ytiles = m.ytiles;
    esc.mapa.tiles = (char*) malloc( esc.mapa.ytiles * esc.mapa.ytiles * sizeof(char));

    if(esc.mlist != -1) {
        glDeleteLists(esc.mlist,1);
    }
    // luego creamos mapa
    esc.mlist = glGenLists(1);
    glNewList(esc.mlist,GL_COMPILE);

    // suelo
    glBegin(GL_POLYGON);
        glColor3f(0.1,0.8,0.1);
        glNormal3f(0,0,1);
        glVertex3f(0,0,0);
        glVertex3f(m.xtiles,0,0);
        glVertex3f(m.xtiles,m.ytiles,0);
        glVertex3f(0,m.ytiles,0);
    glEnd();

    // calcular alturas mapa
    for(y = 0 ; y < m.ytiles ; y++) {
        for(x = 0 ; x < m.xtiles ; x++) {
            c = m.tiles[y * m.xtiles + x];
            if(c >= '1' && c <= '9') {
                esc.mapa.tiles[y * m.xtiles + x] = c - '0';
            } else {
                esc.mapa.tiles[y * m.xtiles + x] = 0;
            }
        }
    }

    // generar cubos mapa
    for(y = 0 ; y < m.ytiles ; y++) {
        for(x = 0 ; x < m.xtiles ; x++) {
            t = get_altura_terreno(x,y);
            if(t>0) {
                // top
                glBegin(GL_POLYGON);
                    glColor3f(0.1,0.8,0.1);
                    glNormal3f(0,0,1);
                    glVertex3f(x,y,t);
                    glVertex3f(x+TAM_TILE,y,t);
                    glVertex3f(x+TAM_TILE,y+TAM_TILE,t);
                    glVertex3f(x,y+TAM_TILE,t);
                glEnd();
                // left
                o = get_altura_terreno(x-1,y);
                if(o < t) {
                    glBegin(GL_POLYGON);
                        glColor3f(0.7,0.2,0.2);
                        glNormal3f(-1,0,0);
                        glVertex3f(x,y,t);
                        glVertex3f(x,y+TAM_TILE,t);
                        glVertex3f(x,y+TAM_TILE,o);
                        glVertex3f(x,y,o);
                    glEnd();
                }
                // front
                o = get_altura_terreno(x,y+1);
                if(o < t) {
                    glBegin(GL_POLYGON);
                        glColor3f(0.7,0.2,0.2);
                        glNormal3f(0,1,0);
                        glVertex3f(x,y+TAM_TILE,t);
                        glVertex3f(x+TAM_TILE,y+TAM_TILE,t);
                        glVertex3f(x+TAM_TILE,y+TAM_TILE,o);
                        glVertex3f(x,y+TAM_TILE,o);
                    glEnd();
                }
                // right
                o = get_altura_terreno(x+1,y);
                if(o < t) {
                    glBegin(GL_POLYGON);
                        glColor3f(0.7,0.2,0.2);
                        glNormal3f(1,0,0);
                        glVertex3f(x+TAM_TILE,y+TAM_TILE,t);
                        glVertex3f(x+TAM_TILE,y,t);
                        glVertex3f(x+TAM_TILE,y,o);
                        glVertex3f(x+TAM_TILE,y+TAM_TILE,o);
                    glEnd();
                }
                // back
                o = get_altura_terreno(x,y-1);
                if(o < t) {
                    glBegin(GL_POLYGON);
                        glColor3f(0.7,0.2,0.2);
                        glNormal3f(0,-1,0);
                        glVertex3f(x+TAM_TILE,y,t);
                        glVertex3f(x,y,t);
                        glVertex3f(x,y,o);
                        glVertex3f(x+TAM_TILE,y,o);
                    glEnd();
                }
            }
        }
    }

    glEndList();

}

#endif

