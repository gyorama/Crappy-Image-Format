#include <stdio.h>
#include <jpeglib.h>
#include <png.h>
#include <string.h>
#include "definitelyNotBitMap.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <lzma.h>

int main(int argc, char *argv[]) {

    int channels = 4;

    if (argc < 3) {
        fprintf(stderr, "Usage: %s <input_image> <output_image>\n", argv[0]);
        return 1;
    } else if (argc > 3) {
        for (int i = 3; i < argc; i++) {
            if (strcmp(argv[i], "--no-alpha-channel") == 0) {
                channels = 3;
            }
        }
    }

    FILE *image = fopen(argv[1], "rb");
    FILE *output = fopen(argv[2], "wb");
    if (!image) {
        fprintf(stderr, "Error opening image file.\n");
        return 1;
    }
    if (!output) {
        fprintf(stderr, "Error opening output file.\n");
        fclose(image);
        return 1;
    }

    int32_t width, height;

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
        .magic = {0x0D42, 0x0A55, 0x0afe, 0x043b, 0x0111, 0x0d34, 0x01bb, 0x0fff},
        .width = width,
        .height = height,
        .channels = channels,
        .depth = 24 // assume true color depth for now. TODO: implement proper depth detection
    };

    unsigned char *imageData = stbi_load(argv[1], &width, &height, NULL, channels);

    if (imageData == NULL) {
        fprintf(stderr, "Error loading image: %s\n", stbi_failure_reason());
        fclose(image);
        fclose(output);
        return 1;
    }

    fwrite(&header, sizeof(header), 1, output);
    fwrite(imageData, width * height * channels, 1, output);
    
    stbi_image_free(imageData);

    fclose(image);
    fclose(output);

    return 0;
}
