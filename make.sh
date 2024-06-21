gcc -Wall -Wextra -O3 -c -FPIC src/definitelyNotBitMap.c -o definitelyNotBitMap.o -ljpeg -lpng
ar rcs libnotbitmap.a definitelyNotBitMap.o

gcc -Wall -Wextra -O3 -L./ src/main.c -o build/pnjpg2cif -lnotbitmap -ljpeg -lpng