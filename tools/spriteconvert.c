#include <stdio.h>
#include <stdint.h>

#define byte unsigned char
#define STANDING_FRAME_COUNT 1
#define STANDING_FRAME_WIDTH 6
#define STANDING_FRAME_HEIGHT 8

/* Piskel data for "standing" */

static const uint32_t standing_data[1][48] = {
{
0xff000000, 0xff000000, 0xffffffff, 0xffffffff, 0xff000000, 0xff000000, 
0xff000000, 0xff000000, 0xffffffff, 0xffffffff, 0xff000000, 0xff000000, 
0xff000000, 0xff000000, 0xffffffff, 0xff000000, 0xff000000, 0xff000000, 
0xff000000, 0xff000000, 0xffffffff, 0xffffffff, 0xff000000, 0xff000000, 
0xff000000, 0xff000000, 0xffffffff, 0xff000000, 0xff000000, 0xff000000, 
0xff000000, 0xff000000, 0xffffffff, 0xff000000, 0xff000000, 0xff000000, 
0xff000000, 0xff000000, 0xffffffff, 0xff000000, 0xff000000, 0xff000000, 
0xff000000, 0xff000000, 0xffffffff, 0xffffffff, 0xff000000, 0xff000000
}
};

byte mask[6]={
  0b100000, //32
  0b010000, //16
  0b001000, //8
  0b000100, //4
  0b000010, //2
  0b000001, //1
};

byte f1[8];
byte f2[8];

int main()
{
    byte one = 64; byte two = 64;
    for(int k=0;k<STANDING_FRAME_COUNT;k++) {
        for(int j=0;j<STANDING_FRAME_HEIGHT;j++) {
            for(int i=0;i<STANDING_FRAME_WIDTH;i++) {
                if(standing_data[k][j*STANDING_FRAME_WIDTH+i] == 0xffffffff) {
                    putchar('*');
                    if(i<6) one = one|mask[i];
                    else two = two|mask[i-6];
                } else {
                    putchar(' ');
                }
            }
            putchar('\n');
            f1[j] = one;
            f2[j] = two;
            one = 64; two = 64;
        }
        printf("%d,%d,%d,%d,%d,%d,%d,%d\n%d,%d,%d,%d,%d,%d,%d,%d\n",f1[0],f1[1],f1[2],f1[3],f1[4],f1[5],f1[6],f1[7],f2[0],f2[1],f2[2],f2[3],f2[4],f2[5],f2[6],f2[7]);
    }

    return 0;
}
