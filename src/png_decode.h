#ifndef __H_PNG_DECODE__
#define __H_PNG_DECODE__

#include <stdint.h>
#include "bmp_encode.h"

// PNG color type
#define PNG_COLOR_TYPE_RGB  2
#define PNG_COLOR_TYPE_RGBA 6

// PNG header structure
typedef struct {
  int32_t width;
  int32_t height;
  uint8_t bit_depth;
  uint8_t color_type;
  uint8_t compression_method;
  uint8_t filter_method;
  uint8_t interlace_method;
} PNG_HEADER;

// PNG decode engine status handle
typedef struct {

  // input parameters
  int32_t input_buffer_size;
  int32_t output_buffer_size;
  int32_t use_high_memory;

  // BMP encoder
  BMP_ENCODE_HANDLE* bmp;

  // png header copy
  PNG_HEADER png_header;

  // output scan line
  uint8_t* line_data;

  // actual screen size
  int32_t actual_width;
  int32_t actual_height;
  int32_t pitch;

  // current decode state
  int32_t current_x;
  int32_t current_y;
  int32_t current_filter;

  // for filter use
  uint8_t left_rf;
  uint8_t left_gf;
  uint8_t left_bf;
  uint8_t* up_rf_ptr;
  uint8_t* up_gf_ptr;
  uint8_t* up_bf_ptr;  

  // RGB888 to RGB555 color map
  uint16_t* rgb555_r;
  uint16_t* rgb555_g;
  uint16_t* rgb555_b;

} PNG_DECODE_HANDLE;

// prototype declarations
void png_decode_init(PNG_DECODE_HANDLE* png);
void png_decode_set_bmp_encoder(PNG_DECODE_HANDLE* png, BMP_ENCODE_HANDLE* bmp);
void png_decode_set_header(PNG_DECODE_HANDLE* png, PNG_HEADER* png_header);
void png_decode_close(PNG_DECODE_HANDLE* png);
int32_t png_decode_exec(PNG_DECODE_HANDLE* png, const uint8_t* png_file_name);

#endif