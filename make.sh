#!/bin/bash

# Why the long file names

gcc -Wall -Wextra -O3 -c -FPIC src/definitelyNotBitMap.c -o definitelyNotBitMap.o -ljpeg -lpng -lm
ar rcs libnotbitmap.a definitelyNotBitMap.o

gcc -Wall -Wextra -O3 -L./ src/main.c -o build/pnjpg2cif -lnotbitmap -ljpeg -lpng -lstb -lm
gcc -Wall -Wextra -O3 -L./ src/displayImage.c -o build/displayImage -lnotbitmap -ljpeg -lpng -lstb -lSDL2 -lm