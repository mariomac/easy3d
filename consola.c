#include<GL/gl.h>
#include<string.h>
#include<stdlib.h>
#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<poll.h>
#include "consola.h"

void inicia_consola(int ancho, int alto, tconsola *con) {
    con->cursorX = 0;
    con->cursorY = 0;
    con->width = ancho;
    con->height = alto;
    con->bytes = (short*) malloc(ancho*alto * sizeof(short));
    con->color = COLOR_BLANCO;
    limpia_consola(con);
    // pipes unix console stdout to this console
    printf("\r"); // por alguna extraña razon, la consola no funciona si no hago el pipe con algo en el buffer
    pipe(con->pipefd);
    dup2(con->pipefd[1],1); // redirect stdout

}

void limpia_consola(tconsola *con) {
    int i; unsigned char p; int pi = 0;
    for (i = 0; i < con->width*con->height; i++) {
        con->bytes[i]=CON_BG;
//        if(i%8==0) {
//            if(i%con->width<FBMPW) {
//                p = fontBitmap[pi];
//                pi++;
//            }
//            else p = 0;
//        }
//        con->bytes[i] = p & 0b10000000 ? con->color : CON_BG;
        p<<=1;
    }    
}

void scroll_up(tconsola *con, int pixels) {
    memcpy(con->bytes, &(con->bytes[pixels*con->width]), (con->height-pixels)*con->width*sizeof(short));
//    int x, y;
//    for(y = 0 ; y < con->height - pixels ; y++) {
//        for(x = 0 ; x < con->width ; x++) {
//            con->bytes[x+y*con->width] = con->bytes[x+(y+pixels)*con->width];
//        }
//    }
    
}

void actualiza_output(tconsola *con) {
    struct pollfd fd;
    fd.fd = con->pipefd[0];
    fd.events = POLLIN | POLLPRI;
    
    
    char reading_buf[1];
    int res;
    while ( (res = poll(&fd,1,0)) != 0) {
        read(con->pipefd[0], reading_buf, 1);
        escribe_char(con, reading_buf[0]);
    }
}

void destruye_consola(tconsola *con) {
    free(con->bytes);
}


void escribe_char(tconsola *con, char ch) {
    int x, y, srcx, srcy;
    unsigned char line;
    unsigned char mask;
    int charsPerLine;
    short pixelColor;
    if(ch=='\n' || con->cursorX + CHARW > con->width) {
        con->cursorX = 0;
        con->cursorY += CHARH;
        if(con->cursorY+CHARH > con->height) {
            scroll_up(con, con->cursorY+CHARH-con->height);
            con->cursorY = con->height - CHARH;
        }
    }
    if(ch >= INIT_CHAR && ch <= END_CHAR ) {
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

void escribe_texto(tconsola *con, char *txt) {
    int i=0;
    while(txt[i] != '\0') {
        escribe_char(con,txt[i]);
        i++;
    }
}
