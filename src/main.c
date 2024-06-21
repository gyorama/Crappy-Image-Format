#include <stdio.h>
#include <jpeglib.h>
#include <png.h>
#include <string.h>
#include "definitelyNotBitMap.h"
#include <stb/stb.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {

    if (argc < 3) {
        fprintf(stderr, "Usage: %s <input_image> <output_image>\n", argv[0]);
        return 1;
    }

    FILE *image = fopen(argv[1], "rb");
    int output = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (!image) {
        fprintf(stderr, "Error opening image file.\n");
        return 1;
    }

    uint32_t width, height;

    char *fileExtension = &argv[1][strlen(argv[1]) - 4];

    if (strcmp(fileExtension, ".jpg") == 0 || strcmp(fileExtension, "jpeg") == 0) {
        getJPEGDimensions(image, &width, &height);
    } else if (strcmp(fileExtension, ".png") == 0) {
        getPNGDimensions(image, &width, &height);
    } else {
        fprintf(stderr, "Unsupported image format.\n"
                        "Try to change the file extension to '.jpg'"
                        " ('.jpeg') or '.png' and see if it'll work\n");
        return 1;
    }

    definitelyNotBitMapHeader header = {
        .magic = {0x4D42, 0xAA55, 0xcafe, 0x443b, 0x0111, 0x0d34, 0x11bb, 0xffff},
        .width = width,
        .height = height,
        .depth = 24 // assume true color depth for now. TODO: implement proper depth detection
    };
    
    write(output, &header, sizeof(definitelyNotBitMapHeader));
    
    fclose(image);

    return 0;
}
