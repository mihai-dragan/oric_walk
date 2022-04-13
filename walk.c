#include <time.h>
#include <conio.h>
#include <string.h>
#include <atmos.h>

#define byte unsigned char

void setup() {
    byte* addr;
    byte value = *(char*)0xC800;
    if(value)
            asm("jsr $EC33");       // HIRES in Atmos (ROM 1.1)
    else
            asm("jsr $E9BB");       // HIRES in Oric-1 (ROM 1.0)
    memset((char*)0xBF68, 0, 120);  // Clear lower text area
    addr = (byte*)0x24E;        //
    *addr = 5;                  // set keyb delay at #24E
    addr = (byte*)0x26A;        //
    *addr = 11;                 // disable keyb click with bitflag at #26A
}

byte* chr = (byte*)0x9900;  // pointer to ASCII characters
byte* achr = (byte*)0x9d00; // pointer to alternative characters

byte pos = 0;
byte walk[8][8] = {         // 4 frames, 2 bytes wide (12pixels), 8 pixels high, sprite animation
{ 70,70,68,78,85,74,74,81 },
{ 64,64,64,64,64,64,64,64 },
{ 64,65,65,65,67,69,66,68 },
{ 64,96,96,64,112,64,112,80 },
{ 64,64,64,64,64,64,64,64 },
{ 88,88,80,120,112,88,120,80 },
{ 64,64,64,64,64,64,64,64 },
{ 88,88,80,120,112,88,84,84 }
};
byte stand[8] = { 76,76,72,76,72,72,72,76 };
byte eraser[8] = { 64,64,64,64,64,64,64,64 };
byte flipmap[8] = { 0,4,2,6,1,5,3,7 };
byte faceright = 1;

void draw_spr(byte spr[8], int haddr) {
    byte i;
    for(i=0;i<8;i++) {
        unsigned char* addr = (unsigned char*)haddr;
        *addr = spr[i];
        haddr=haddr+40;
    }
}

// flip the six bits holding the pixels
byte flip(byte b) {
    byte r=b&0b00111111;
    r=(flipmap[r&0b111] << 3) | flipmap[r>>3];
    return r|0b01000000;
}

void draw_sprR(byte spr[8], int haddr) {
    byte i, inv;
    for(i=0;i<8;i++) {
        byte* addr = (byte*)haddr;
        inv = flip(spr[i]);
        *addr = inv;
        haddr=haddr+40;
    }
}

void draw_char(byte cnum, int haddr) {
    byte i;
    byte offst = cnum<<3;
    for(i=0;i<8;i++) {
        byte* addr = (byte*)haddr;
        *addr = 64+chr[offst+i];
        haddr=haddr+40;
    }
}

void draw_achar(byte cnum, int haddr) {
    byte i;
    byte offst = cnum<<3;
    for(i=0;i<8;i++) {
        byte* addr = (byte*)haddr;
        *addr = 64+achr[offst+i];
        haddr=haddr+40;
    }
}

void delay() {
    clock_t goal;
    goal = clock () + CLOCKS_PER_SEC/15;
    while ((long) (goal - clock ()) > 0) ;
}

void move_right() {
    byte i = 0;
    if(!faceright) {
        faceright=1;
        pos=pos-1;
    }
    if(pos<38) {
        pos=pos+1;
        for(i=0;i<4;i++) {
                draw_spr(walk[i*2],0xa000+pos);
                draw_spr(walk[i*2+1],0xa001+pos);
                delay();
        }
    }
}

void move_left() {
    byte i = 0;
    if(faceright) {
        faceright=0;
        pos=pos+1;
    }
    if(pos>1) {
        pos=pos-1;
        for(i=0;i<4;i++) {
                draw_sprR(walk[i*2],0xa001+pos);
                draw_sprR(walk[i*2+1],0xa000+pos);
                delay();
        }
    }
}

void main() {
    byte c;
    setup();
    while(1) {
        if(faceright) {
            draw_spr(eraser,0xa000+pos);
            draw_spr(stand,0xa001+pos);
        } else {
            draw_spr(eraser,0xa001+pos);
            draw_sprR(stand,0xa000+pos);
        }
        delay();
        
        if(kbhit()>0) {
            c = cgetc();
            if(c==CH_ESC) return;
            if(c==CH_CURS_RIGHT) move_right();
            if(c==CH_CURS_LEFT) move_left();
            /*if(c==CH_CURS_UP) move_up();
            if(c==CH_CURS_DOWN) move_down();*/
        }
    }
}
