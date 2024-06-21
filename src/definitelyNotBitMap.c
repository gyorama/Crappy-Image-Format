#include "definitelyNotBitMap.h"
#include <stdlib.h>
#include <jpeglib.h>
#include <png.h>
#include <stdio.h>
#include <string.h>

int getJPEGDimensions(FILE *image, uint32_t *width, uint32_t *height) {
    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;

    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_decompress(&cinfo);
    jpeg_stdio_src(&cinfo, image);
    jpeg_read_header(&cinfo, TRUE);

    jpeg_start_decompress(&cinfo);

    JSAMPARRAY row_pointer = (*cinfo.mem->alloc_sarray)((j_common_ptr)&cinfo, JPOOL_IMAGE, cinfo.output_width, 1);

    memcpy(width, &cinfo.output_width, sizeof(uint32_t));
    memcpy(height, &cinfo.output_height, sizeof(uint32_t));

    while (cinfo.output_scanline < cinfo.output_height) {
        jpeg_read_scanlines(&cinfo, row_pointer, 1);
    }

    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);
    return 0;
}

int getPNGDimensions(FILE *image, uint32_t *width, uint32_t *height) {

    png_structp png_ptr;
    png_infop info_ptr;

    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        return 1;
    }

    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return 1;
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 1;
    }

    png_init_io(png_ptr, image);

    png_read_info(png_ptr, info_ptr);

    *width = png_get_image_width(png_ptr, info_ptr);
    *height = png_get_image_height(png_ptr, info_ptr);
    
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

    return 0;
}