#ifndef CONSOLA_H
#define	CONSOLA_H

#include "bitmaps.h"

#define CHARW 8
#define CHARH 16

#define INIT_CHAR ' '
#define END_CHAR 'z'

#define CON_BG 0x0009

#define COLOR_BLANCO 0xffff
#define COLOR_ROJO 0xf00f
#define COLOR_AZUL 0x00ff
#define COLOR_VERDE 0x0f0f
#define COLOR_AMARILLO 0xff0f

#define FACTOR_ESCALA_CONSOLA 2
#define ALTO_CONSOLA 0.25

typedef struct {
    GLuint textura;
    int width;
    int height;
    int cursorX;
    int cursorY;
    short color;
    unsigned short *bytes;
    int pipefd[2];
} tconsola;

void actualiza_output(tconsola *con);
void inicia_consola(int ancho, int alto, tconsola *con);
void destruye_consola(tconsola *con);
void escribe_char(tconsola *con, char ch);
void limpia_consola(tconsola *con);
void escribe_texto(tconsola *con, char *txt);

#endif	/* CONSOLA_H */

