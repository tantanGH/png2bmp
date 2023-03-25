#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "himem.h"
#include "bmp_encode.h"

//
//  init BMP encode handler
//
int32_t bmp_encode_init(BMP_ENCODE_HANDLE* bmp, FILE* fp) {

  bmp->fp = fp;

  bmp->width = 0;
  bmp->height = 0;
  bmp->padding_bytes = 0;
  
  memset(bmp->padding, 0, 4);

  return 0;
}

//
//  close BMP encode handler
//
void bmp_encode_close(BMP_ENCODE_HANDLE* bmp) {
}

//
//  write BMP header
//
int32_t bmp_encode_write_header(BMP_ENCODE_HANDLE* bmp, uint32_t width, uint32_t height) {

  int32_t rc = -1;

  if (bmp == NULL || bmp->fp == NULL) goto exit;

  bmp->width = width;
  bmp->height = height;

  bmp->padding_bytes = (4 - ((bmp->width * 3) % 4)) % 4;

  static uint8_t bmp_header[ BMP_HEADER_BYTES ];

  memset(bmp_header, 0, BMP_HEADER_BYTES);

  // eye catch
  bmp_header[0] = 0x42;
  bmp_header[1] = 0x4d;

  // file size
  uint32_t file_size = 54 + width * height * 3;
  bmp_header[2] = file_size & 0xff;
  bmp_header[3] = (file_size >> 8) & 0xff;
  bmp_header[4] = (file_size >> 16) & 0xff;
  bmp_header[5] = (file_size >> 24) & 0xff;

  // header size
  bmp_header[10] = 54;

  // information size
  bmp_header[14] = 40;

  // width
  bmp_header[18] = bmp->width & 0xff;
  bmp_header[19] = (bmp->width >> 8) & 0xff;
  bmp_header[20] = (bmp->width >> 16) & 0xff;
  bmp_header[21] = (bmp->width >> 24) & 0xff;
  
  // height
  bmp_header[22] = bmp->height & 0xff;
  bmp_header[23] = (bmp->height >> 8) & 0xff;
  bmp_header[24] = (bmp->height >> 16) & 0xff;
  bmp_header[25] = (bmp->height >> 24) & 0xff;

  // planes
  bmp_header[26] = 1;

  // bit depth
  bmp_header[28] = 24;

  rc = fwrite(bmp_header, 1, BMP_HEADER_BYTES, bmp->fp) == BMP_HEADER_BYTES ? 0 : -1;

exit:
  return rc;
}

//
//  encode
//
int32_t bmp_encode_write(BMP_ENCODE_HANDLE* bmp, uint32_t pos_y, uint8_t* bmp_buffer, size_t bmp_buffer_line_bytes, size_t bmp_buffer_lines) {
  
  int32_t rc = 0;

  fseek(bmp->fp, BMP_HEADER_BYTES + (bmp->width * 3 + bmp->padding_bytes) * (bmp->height - (pos_y + bmp_buffer_lines)), SEEK_SET);
  for (int16_t i = bmp_buffer_lines - 1; i >= 0; i--) {
    size_t len = fwrite(bmp_buffer + bmp->width * 3 * i, 1, bmp_buffer_line_bytes, bmp->fp);
    if (len != bmp_buffer_line_bytes) {
      rc = -1;
      break;
    }
    if (bmp->padding_bytes > 0) {
      fwrite(bmp->padding, 1, bmp->padding_bytes, bmp->fp);
    }
  }

  return rc;
}