#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#ifndef DEFINITELY_NOT_BITMAP_H
#define DEFINITELY_NOT_BITMAP_H

typedef struct definitelyNotBitMap {
    uint16_t magic[8];
    int32_t width;
    int32_t height;
    uint8_t channels;
    uint16_t depth;
} definitelyNotBitMapHeader;

int getJPEGDimensions(FILE *image, int32_t *width, int32_t *height);
int getPNGDimensions(FILE *image, int32_t *width, int32_t *height);
unsigned char *loadImage(FILE *image, definitelyNotBitMapHeader *header);

#endif