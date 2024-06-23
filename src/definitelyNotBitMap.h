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

typedef struct BitMap {
    uint16_t type;
    uint32_t size;
    uint16_t reserved1;
    uint16_t reserved2;
    uint32_t offset;
    uint32_t headerSize;
    uint32_t width;
    uint32_t height;
    uint16_t channels;
    uint16_t depth;
    uint32_t colorMode;
    uint32_t imageSize;

} BitMapHeader;

int getJPEGDimensions(FILE *image, int32_t *width, int32_t *height);
int getPNGDimensions(FILE *image, int32_t *width, int32_t *height);
unsigned char *loadImage(FILE *image, definitelyNotBitMapHeader *header);

#endif