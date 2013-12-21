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
#define COLOR_AMARILLO 0x0fff

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
} tconsola;

void inicia_consola(int ancho, int alto, tconsola *con);
void destruye_consola(tconsola *con);
void escribe_char(char ch, tconsola *con);
void limpia_consola(tconsola *con);

#endif	/* CONSOLA_H */

