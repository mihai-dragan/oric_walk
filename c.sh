rm *.map
rm *.s
rm *.o

#cc65 -Oirs --codesize 500 -T -g -t atmos walk.c
cc65 -Cl -Ois --codesize 500 -t atmos walk.c
ca65 walk.s
ld65 -o walk.tap -t atmos -m walk.map walk.o atmos.lib
