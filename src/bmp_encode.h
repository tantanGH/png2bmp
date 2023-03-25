#ifndef __H_BMP_DECODE__
#define __H_BMP_DECODE__

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>

#define BMP_HEADER_BYTES (54)

typedef struct {
  int16_t half_size;
  uint32_t width;
  uint32_t height;
  size_t padding_bytes;
  uint8_t padding[4];
  FILE* fp;
} BMP_ENCODE_HANDLE;

int32_t bmp_encode_init(BMP_ENCODE_HANDLE* bmp, FILE* fp, int16_t half_size);
void bmp_encode_close(BMP_ENCODE_HANDLE* bmp);
int32_t bmp_encode_write_header(BMP_ENCODE_HANDLE* bmp, uint32_t width, uint32_t height);
int32_t bmp_encode_write(BMP_ENCODE_HANDLE* bmp, uint32_t pos_y, uint8_t* bmp_buffer, size_t bmp_buffer_line_bytes, size_t bmp_buffer_lines);

#endif