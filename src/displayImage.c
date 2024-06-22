#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <SDL2/SDL.h>
#include "definitelyNotBitMap.h"
#include <unistd.h>
#include <fcntl.h>
#include <stdbool.h>
#include <lzma.h>

int main(int argc, char *argv[]) {

    definitelyNotBitMapHeader header;
    uint16_t magic[8] = {0x0D42, 0x0A55, 0x0afe, 0x043b, 0x0111, 0x0d34, 0x01bb, 0x0fff};

    if (argc < 2) {
        fprintf(stderr, "Usage: %s <input_image>\n", argv[0]);
        return 1;
    }

    FILE *image = fopen(argv[1], "rb");
    if (!image) {
        fprintf(stderr, "Error opening image file.\n");
        return 1;
    }
    fread(&header, sizeof(header), 1, image);
    if (memcmp(header.magic, magic, 8) != 0) {
        fprintf(stderr, "Wrong sign.\n");
        fclose(image);
        return 1;
    }

    // begin to do SDL stuff
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        fclose(image);
        return 1;
    }

    // Create window
    SDL_Window* window = SDL_CreateWindow("CIF image viewer",
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED,
                                          header.width, header.height,
                                          SDL_WINDOW_SHOWN);
    if (window == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_Quit();
        fclose(image);
        return 1;
    }

    // Create renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        fclose(image);
        return 1;
    }

    unsigned char *imageData = loadImage(image, &header);

    if (!imageData) {
        fprintf(stderr, "Error loading image\n");
        fclose(image);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);

    for (int32_t y = 0; y < header.height; ++y) {
        for (int32_t x = 0; x < header.width; ++x) {
            int pixel = (y * header.width + x) * header.channels;
            SDL_SetRenderDrawColor(renderer, imageData[pixel], imageData[pixel + 1], imageData[pixel + 2], (header.channels == 4) ? imageData[pixel + 3] : SDL_ALPHA_OPAQUE);
            SDL_RenderDrawPoint(renderer, x, y);
        }
    }

    SDL_RenderPresent(renderer);

    // Event handler
    SDL_Event e;
    bool isRunning = true;

    // Main application loop
    while (isRunning) {
        // Handle events on the queue
        while (SDL_PollEvent(&e)) {
            switch (e.type) {
                case SDL_QUIT:
                    isRunning = false;
                    break;
            }
        }

        SDL_Delay(33);
    }

    // Destroy renderer
    free(imageData);
    SDL_DestroyRenderer(renderer);
    // Destroy window
    SDL_DestroyWindow(window);
    // Quit SDL subsystems
    SDL_Quit();

    return 0;
}
