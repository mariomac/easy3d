#ifndef EASY3D_H
#define EASY3D_H

#include<SDL2/SDL_opengl.h>
#include<SDL2/SDL.h>
#include<GL/gl.h>
#include<GL/glu.h>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>

#define ZNEAR 0.0001
#define ZFAR 50

#define FOVY 60

#define FPS 30.0

#define TECLA_ESC 0b1
#define TECLA_ARRIBA 2
#define TECLA_ABAJO 4
#define TECLA_IZDA 8
#define TECLA_DCHA 16

#define TAM_TILE 1.0
#define TAM_PATRON 0.2

#define PARED '#'
#define SUELO '0'

typedef struct {
    double posX, posY, posZ;
    double anguloX;
} tcamara;

typedef struct {
    int ancho, alto;
    SDL_Window *wnd;
    SDL_GLContext glctx;
    int teclas;
} tventana;

typedef struct {
    int xtiles, ytiles;
    char *paredes;
} tmapa;

typedef struct {
    GLuint mlist;
    tmapa mapa;
} tescenario;

void cubo(double lado, double cx, double cy, double cz);

tventana ventana;
tescenario esc = {-1};

void abre_ventana() {
    //stderr = fopen("err.txt","w");
    fprintf(stderr, "ATENCION: este archivo solo esta para pillar algunos mensajes del sistema."
            " Ignoralos si no sabes que quieren decir\n");
    if (SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO) == -1) {
        fprintf(stderr, "Ocurrio un error al iniciar programa: %s\n", SDL_GetError());
        exit(-1);
    }
    ventana.teclas = 0;
    ventana.ancho = 640;
    ventana.alto = 480;
    ventana.wnd = SDL_CreateWindow("Easy3D window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            ventana.ancho, ventana.alto, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

    //Create context
    if (!ventana.wnd) {
        printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
        exit(-1);
    }
    ventana.glctx = SDL_GL_CreateContext(ventana.wnd);
    if (ventana.glctx == NULL) {
        printf("OpenGL context could not be created! SDL Error: %s\n", SDL_GetError());
        exit(-1);
    } else {
        //Use Vsync
        if (SDL_GL_SetSwapInterval(1) < 0) {
            fprintf(stderr, "Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());
        }

        //Initialize OpenGL


    }

    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, ventana.ancho, ventana.alto);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(FOVY, (double) -ventana.ancho / (double) ventana.alto, ZNEAR, ZFAR);
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
    free(esc.mapa.paredes);
}

struct timespec startFrameTime = {0, 0};

void muestra_fotograma(tcamara cam) {
    long difMilis, difseconds;
    struct timespec now;
    clock_gettime(CLOCK_BOOTTIME, &now);
    difseconds = (now.tv_sec - startFrameTime.tv_sec);
    difMilis = difseconds * 1000 + (long) (now.tv_nsec - startFrameTime.tv_nsec) / 1000000L; // convert to ms
    //printf("%ld\n", difMilis);
    if (difMilis >= 0 && difMilis < 1000.0 / FPS) {
        SDL_Delay((1000.0 / FPS)); //-difNanos);
    }
    clock_gettime(CLOCK_BOOTTIME, &startFrameTime);


    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    gluLookAt(cam.posX, cam.posY, cam.posZ,
            cam.posX + sin(cam.anguloX),
            cam.posY - cos(cam.anguloX),
            cam.posZ,
            0, 0, 1);
    // Multi-colored side - FRONT

    GLfloat lightcol[] = {0.3, 0.3, 0.3, 0};
    GLfloat ambient[] = {0.1, 0.1, 0.1, 0};

    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT1, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightcol);
    glLightfv(GL_LIGHT1, GL_SPECULAR, lightcol);
    //glLightfv(GL_LIGHTn, GL_SPECULAR, lightcol );
    GLfloat lightpos[] = {20, 10, 10, 0};
    glLightfv(GL_LIGHT0, GL_POSITION, lightpos);
    GLfloat lightpos2[] = {-20, 10, 10, 0};
    glLightfv(GL_LIGHT1, GL_POSITION, lightpos2);

    glCallList(esc.mlist);

    glFlush();

    SDL_GL_SwapWindow(ventana.wnd);

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
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
        } else if (event.type = SDL_KEYUP) {
            switch (event.key.keysym.sym) {
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
    double n = -lado / 2.0, p = lado / 2.0;
    glPushMatrix();
    glTranslated(cx, cy, cz);
    //frente
    glColor3f(1, 0, 0);

    GLUquadric *c = gluNewQuadric();
    gluSphere(c, lado / 2, 3, 3);
    gluDeleteQuadric(c);

    glPopMatrix();
}

int tecla_pulsada(int tecla) {
    return ventana.teclas & tecla;
}

char que_hay_aqui(tmapa mapa, double x, double y) {
    int xx = (int) x;
    int yy = (int) y;
    if (xx >= 0 && xx < mapa.xtiles && yy >= 0 && yy < mapa.ytiles) {
        return mapa.paredes[yy * mapa.xtiles + xx];
    } else {
        return PARED;
    }
}

/* las tiles multiplican por 10 en el mapa real */
#define WALL_HIGH 1.0

tcamara lee_mapa(tmapa mapa) {
    tcamara cam = {0, 0, 0, 0};
    GLuint sueloTex, ladrilloTex, techoTex;
    int x, y, i, j, k;
    char c, c1;
    int cx, cy;
    double t, o;
    esc.mapa.xtiles = mapa.xtiles;
    esc.mapa.ytiles = mapa.ytiles;
    esc.mapa.paredes = (char*) malloc(mapa.xtiles * mapa.ytiles * sizeof (char));

    // calcular alturas mapa
    for (y = 0; y < mapa.ytiles; y++) {
        for (x = 0; x < mapa.xtiles; x++) {
            c = mapa.paredes[y * mapa.xtiles + x];
            esc.mapa.paredes[y * mapa.xtiles + x] = c;
            if (c == 'p' || c == 'P') {
                cam.posX = x + 0.5;
                cam.posY = y + 0.5;
            }
        }
    }

    if (esc.mlist != -1) {
        glDeleteLists(esc.mlist, 1);
    }
    // luego creamos mapa
    esc.mlist = glGenLists(1);
    glNewList(esc.mlist, GL_COMPILE);

#define NN 0x0000
#define G0 0x3330
#define G1 0x6660
#define G2 0xaaa0
    //generar texturas
    int width = 8;
    int height = 8;
    unsigned short suelo[] = {
        NN, NN, G0, G0, G0, G0, G0, NN,
        NN, G0, G2, G2, G1, G1, G1, G0,
        G0, G2, G2, G1, G1, G1, G1, G1,
        G0, G2, G1, G1, G1, G1, G1, G1,
        G0, G1, G1, G1, G1, G1, G1, G1,
        G0, G1, G1, G1, G1, G1, G1, G1,
        G0, G1, G1, G1, G1, G1, G1, G1,
        NN, G0, G1, G1, G1, G1, G1, G0,
    };
    glGenTextures(1, &sueloTex);
    glBindTexture(GL_TEXTURE_2D, sueloTex);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_SHORT_4_4_4_4, suelo);

#define R0 0x3100
#define R1 0x6210
#define R2 0xa420
    unsigned short ladrillo[] = {
        NN, NN, NN, NN, NN, NN, NN, NN,
        NN, R2, R2, R1, R1, R1, R1, R1,
        NN, R2, R1, R1, R1, R1, R1, R0,
        NN, R1, R1, R1, R1, R1, R0, R0,
        NN, NN, NN, NN, NN, NN, NN, NN,
        R1, R1, R1, R0, NN, R2, R2, R1,
        R1, R1, R1, R0, NN, R2, R1, R1,
        R1, R1, R0, R0, NN, R1, R1, R1,
    };

    glGenTextures(1, &ladrilloTex);
    glBindTexture(GL_TEXTURE_2D, ladrilloTex);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_SHORT_4_4_4_4, ladrillo);

#define Y1 0xec00
#define Y2 0xfd10
    int tsize = 16;
    unsigned short techo[] = {
        G0, G0, G0, G0, G0, G0, G0, G0, G0, G0, G0, G0, G0, G0, G0, G0,
        G0, G0, G0, G0, G0, G0, G0, G0, G0, G0, G0, G0, G0, G0, G0, G0,
        G0, G0, G0, G0, G0, G0, G0, G0, G0, G0, G0, G0, G0, G0, G0, G0,
        G0, G0, G0, G0, G0, G0, G0, G0, G0, G0, G0, G0, G0, G0, G0, G0,
        G0, G0, G0, G0, G0, G0, G0, G0, G0, G0, G0, G0, G0, G0, G0, G0,
        G0, G0, G0, G0, G0, G0, G0, G0, G0, G0, G0, G0, G0, G0, G0, G0,
        G0, G0, G0, G0, G0, G0, G0, Y1, Y1, Y1, G0, G0, G0, G0, G0, G0,
        G0, G0, G0, G0, G0, G0, Y1, Y2, Y2, Y2, Y1, G0, G0, G0, G0, G0,
        G0, G0, G0, G0, G0, G0, Y1, Y2, Y2, Y2, Y1, G0, G0, G0, G0, G0,
        G0, G0, G0, G0, G0, G0, Y1, Y2, Y2, Y2, Y1, G0, G0, G0, G0, G0,
        G0, G0, G0, G0, G0, G0, G0, Y1, Y1, Y1, G0, G0, G0, G0, G0, G0,
        G0, G0, G0, G0, G0, G0, G0, G0, G0, G0, G0, G0, G0, G0, G0, G0,
        G0, G0, G0, G0, G0, G0, G0, G0, G0, G0, G0, G0, G0, G0, G0, G0,
        G0, G0, G0, G0, G0, G0, G0, G0, G0, G0, G0, G0, G0, G0, G0, G0,
        G0, G0, G0, G0, G0, G0, G0, G0, G0, G0, G0, G0, G0, G0, G0, G0,
        G0, G0, G0, G0, G0, G0, G0, G0, G0, G0, G0, G0, G0, G0, G0, G0,
    };

    glGenTextures(1, &techoTex);
    glBindTexture(GL_TEXTURE_2D, techoTex);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tsize, tsize, 0, GL_RGBA, GL_UNSIGNED_SHORT_4_4_4_4, techo);

    glColor3f(1, 1, 1);
    //techo
    // suelo
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, techoTex);

    glBegin(GL_POLYGON);
    glNormal3f(0, 0, 1);
    glTexCoord2d(0, mapa.ytiles);
    glVertex3f(0, mapa.ytiles, TAM_TILE);
    glTexCoord2d(mapa.xtiles, mapa.ytiles);
    glVertex3f(mapa.xtiles, mapa.ytiles, TAM_TILE);
    glTexCoord2d(mapa.xtiles, 0);
    glVertex3f(mapa.xtiles, 0, TAM_TILE);
    glTexCoord2d(0, 0);
    glVertex3f(0, 0, TAM_TILE);

    glEnd();

    // suelo
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, sueloTex);

    glBegin(GL_POLYGON);
    glNormal3f(0, 0, 1);
    glTexCoord2d(0, 0);
    glVertex3f(0, 0, 0);
    glTexCoord2d(mapa.xtiles / TAM_PATRON, 0);
    glVertex3f(mapa.xtiles, 0, 0);
    glTexCoord2d(mapa.xtiles / TAM_PATRON, mapa.ytiles / TAM_PATRON);
    glVertex3f(mapa.xtiles, mapa.ytiles, 0);
    glTexCoord2d(0, mapa.ytiles / TAM_PATRON);
    glVertex3f(0, mapa.ytiles, 0);
    glEnd();

    // generar cubos mapa
    for (y = 0; y < mapa.ytiles; y++) {
        for (x = 0; x < mapa.xtiles; x++) {
            c = que_hay_aqui(esc.mapa, x, y);
            if (c == PARED) {
                t = TAM_TILE;
                o = 0;
                // top
                //                glEnable( GL_TEXTURE_2D );
                //    			glBindTexture( GL_TEXTURE_2D, sueloTex );
                //                glBegin(GL_POLYGON);
                //
                //                    glNormal3f(0,0,1);
                //                    glTexCoord2d(0,0);
                //                    glVertex3f(x,y,t);
                //                    glTexCoord2d(1/TAM_PATRON,0);
                //                    glVertex3f(x+TAM_TILE,y,t);
                //                    glTexCoord2d(1/TAM_PATRON,1/TAM_PATRON);
                //                    glVertex3f(x+TAM_TILE,y+TAM_TILE,t);
                //                    glTexCoord2d(0,1/TAM_PATRON);
                //                    glVertex3f(x,y+TAM_TILE,t);
                //                glEnd();
                // left
                c = que_hay_aqui(esc.mapa, x - 1, y);
                if (c != PARED) {
                    glEnable(GL_TEXTURE_2D);
                    glBindTexture(GL_TEXTURE_2D, ladrilloTex);
                    glBegin(GL_POLYGON);
                    glNormal3f(-1, 0, 0);
                    glTexCoord2d(0, 0);
                    glVertex3f(x, y, t);
                    glTexCoord2d(1 / TAM_PATRON, 0);
                    glVertex3f(x, y + TAM_TILE, t);
                    glTexCoord2d(1 / TAM_PATRON, 1 * (t - o) / TAM_PATRON);
                    glVertex3f(x, y + TAM_TILE, o);
                    glTexCoord2d(0, 1 * (t - o) / TAM_PATRON);
                    glVertex3f(x, y, o);
                    glEnd();
                }
                // front
                c = que_hay_aqui(esc.mapa, x, y + 1);
                if (c != PARED) {
                    glEnable(GL_TEXTURE_2D);
                    glBindTexture(GL_TEXTURE_2D, ladrilloTex);
                    glBegin(GL_POLYGON);
                    glNormal3f(0, 1, 0);
                    glTexCoord2d(0, 0);
                    glVertex3f(x, y + TAM_TILE, t);
                    glTexCoord2d(1 / TAM_PATRON, 0);
                    glVertex3f(x + TAM_TILE, y + TAM_TILE, t);
                    glTexCoord2d(1 / TAM_PATRON, 1 * (t - o) / TAM_PATRON);
                    glVertex3f(x + TAM_TILE, y + TAM_TILE, o);
                    glTexCoord2d(0, 1 * (t - o) / TAM_PATRON);
                    glVertex3f(x, y + TAM_TILE, o);
                    glEnd();
                }
                // right
                c = que_hay_aqui(esc.mapa, x + 1, y);
                if (c != PARED) {

                    glEnable(GL_TEXTURE_2D);
                    glBindTexture(GL_TEXTURE_2D, ladrilloTex);
                    glBegin(GL_POLYGON);
                    glNormal3f(1, 0, 0);
                    glTexCoord2d(0, 0);
                    glVertex3f(x + TAM_TILE, y + TAM_TILE, t);
                    glTexCoord2d(1 / TAM_PATRON, 0);
                    glVertex3f(x + TAM_TILE, y, t);
                    glTexCoord2d(1 / TAM_PATRON, 1 * (t - o) / TAM_PATRON);
                    glVertex3f(x + TAM_TILE, y, o);
                    glTexCoord2d(0, 1 * (t - o) / TAM_PATRON);
                    glVertex3f(x + TAM_TILE, y + TAM_TILE, o);
                    glEnd();
                }
                // back
                c = que_hay_aqui(esc.mapa, x, y - 1);
                if (c != PARED) {

                    glEnable(GL_TEXTURE_2D);
                    glBindTexture(GL_TEXTURE_2D, ladrilloTex);
                    glBegin(GL_POLYGON);
                    glNormal3f(0, -1, 0);
                    glTexCoord2d(0, 0);
                    glVertex3f(x + TAM_TILE, y, t);

                    glTexCoord2d(1 / TAM_PATRON, 0);
                    glVertex3f(x, y, t);
                    glTexCoord2d(1 / TAM_PATRON, 1 * (t - o) / TAM_PATRON);
                    glVertex3f(x, y, o);
                    glTexCoord2d(0, 1 * (t - o) / TAM_PATRON);
                    glVertex3f(x + TAM_TILE, y, o);
                    glEnd();
                }
            }
        }
    }

    glDeleteTextures(1, &sueloTex);
    glDeleteTextures(1, &ladrilloTex);
    glDeleteTextures(1, &techoTex);
    glEndList();

    return cam;

}

#endif

