#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct definitelyNotBitMap {
    uint16_t magic[8];
    uint32_t width;
    uint32_t height;
    uint16_t depth;
} definitelyNotBitMapHeader;

int getJPEGDimensions(FILE *image, uint32_t *width, uint32_t *height);
int getPNGDimensions(FILE *image, uint32_t *width, uint32_t *height);
