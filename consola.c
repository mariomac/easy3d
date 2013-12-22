#include<GL/gl.h>
#include<stdlib.h>
#include "consola.h"

void inicia_consola(int ancho, int alto, tconsola *con) {
    con->cursorX = 0;
    con->cursorY = 0;
    con->width = ancho;
    con->height = alto;
    con->bytes = (short*) malloc(ancho*alto * sizeof(short));
    con->color = COLOR_BLANCO;
    limpia_consola(con);
}

void limpia_consola(tconsola *con) {
    int i; unsigned char p; int pi = 0;
    for (i = 0; i < con->width*con->height; i++) {
        if(i%8==0) {
            if(i%con->width<FBMPW) {
                p = fontBitmap[pi];
                pi++;
            }
            else p = 0;
        }
        con->bytes[i] = p & 0b10000000 ? con->color : CON_BG;
        p<<=1;
    }    
}


void destruye_consola(tconsola *con) {
    free(con->bytes);
}


void escribe_char(char ch, tconsola *con) {
    int x, y, srcx, srcy;
    unsigned char line;
    unsigned char mask;
    int charsPerLine;
    short pixelColor;
    if(ch=='\n') {
        con->cursorX = 0;
        con->cursorY += CHARH;
    } else if(ch >= INIT_CHAR && ch <= END_CHAR ) {
        charsPerLine=FBMPW/CHARW;
        ch -= INIT_CHAR;
        srcy = (ch / charsPerLine) * CHARH;
        for(y=0;y<CHARH;y++) {
            srcx = ((ch * CHARW) / 8) % charsPerLine;
            for(x=0;x<CHARW;x++){
                if(x%8==0){
                    line=fontBitmap[srcx+srcy*charsPerLine * CHARW / 8];
                    srcx++;
                }
                if(line&0b10000000) {
                    pixelColor = con->color;
                } else {
                    pixelColor = CON_BG;
                }
                line<<=1;
                con->bytes[(con->cursorX+x)+(con->cursorY+y)*con->width] = pixelColor;
            }
            srcy++;
        }       
        con->cursorX += CHARW;
    }
}
