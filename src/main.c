#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <doslib.h>
#include <iocslib.h>
#include "himem.h"
#include "png_decode.h"
#include "bmp_encode.h"

#define VERSION "0.1.0 (2023/03/25)"

#define MAX_PATH_LEN (255)

//
//  show help message
//
static void show_help_message() {
  printf("usage: png2bmp [options] <file.png>\n");
  printf("options:\n");
  printf("   -o <outfile> ... output filename (default:auto)\n");
  printf("   -s           ... half size scaling\n");
  printf("   -h           ... show help message\n");
}

//
//  main
//
int32_t main(int32_t argc, uint8_t* argv[]) {

  // default return code
  int32_t rc = -1;

  // source file name
  uint8_t* file_name = NULL;

  // output file name
  static uint8_t out_name[ MAX_PATH_LEN + 1 ];
  out_name[0] = '\0';

  // output file pointer
  FILE* fo = NULL;

  // half size option
  int16_t half_size = 0;

  // credit
  printf("PNG2BMP.X - PNG to BMP converter for X680x0 " VERSION " by tantan\n");

  // check command line
  if (argc < 2) {
    show_help_message();
    goto exit;
  }

  // parse command lines
  for (int16_t i = 1; i < argc; i++) {
    if (argv[i][0] == '-' && strlen(argv[i]) >= 2) {
      if (argv[i][1] == 'o') {
        if (i+1 < argc && out_name[0] == '\0') {
          strcpy(out_name, argv[i+1]);
          i++;
        } else {
          show_help_message();
          goto exit;
        }
      } else if (argv[i][1] == 's') {
        half_size = 1;
      } else if (argv[i][1] == 'h') {
        show_help_message();
        goto exit;
      } else {
        printf("error: unknown option (%s).\n",argv[i]);
        goto exit;
      }
    } else {
      if (file_name != NULL) {
        printf("error: only 1 file can be specified.\n");
        goto exit;
      }
      if (strlen(argv[i]) < 5) {
        printf("error: not a png file.\n");
        goto exit;
      }
      file_name = argv[i];
    }
  }

  if (file_name == NULL) {
    show_help_message();
    goto exit;
  }

  if (out_name[0] == '\0') {
    strcpy(out_name, file_name);
    uint8_t* out_ext = out_name + strlen(out_name) - 4;
    uint8_t* file_ext = file_name + strlen(file_name) - 4;
    if (strcmp(file_ext, ".png") == 0) {
      strcpy(out_ext, ".bmp");
    } else {
      strcpy(out_ext, ".BMP");
    }
  }

  printf("source file: %s\n", file_name);
  printf("output file: %s\n", out_name);
  
  // output file overwrite check
  struct FILBUF filbuf;
  if (FILES(&filbuf, out_name, 0x20) >= 0) {
    printf("warning: output file (%s) already exists. overwrite? (y/n)", out_name);
    uint8_t c;
    do {
      c = INKEY();
      if (c == 'n' || c == 'N') {
        printf("\ncanceled.\n");
        goto exit;
      }
    } while (c != 'y' && c != 'Y');
    printf("\n");
  }

  // create output file and reopen it as random write mode
  fo = fopen(out_name, "wb");
  if (fo == NULL) {
    printf("error: failed to open the output file (%s).\n", out_name);
    goto exit;
  }
  fclose(fo);
  fo = fopen(out_name, "rb+");

  BMP_ENCODE_HANDLE bmp_encode = { 0 };
  bmp_encode_init(&bmp_encode, fo);

  PNG_DECODE_HANDLE png_decode = { 0 };
  png_decode_init(&png_decode, half_size);
  png_decode_set_bmp_encoder(&png_decode, &bmp_encode);

  printf("converting...");
  rc = png_decode_exec(&png_decode, file_name);
  printf("done.\n");

  png_decode_close(&png_decode);
  bmp_encode_close(&bmp_encode);

exit:

  if (fo != NULL) {
    fclose(fo);
    fo = NULL;
  }

  return rc;
}