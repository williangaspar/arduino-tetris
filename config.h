#ifndef CONFIG_H
#define CONFIG_H

/*
  The purpose of this file is to hold const values that when modifield should:
  1. Make the game compatible with other screen types and sizes
  2. Alter key game behaviour
*/

#include <Adafruit_ST7735.h>

#define SQR_SIZE 10
#define SQR_GAP 2
#define SQR_TSIZE 12
#define BLOB_W 8
#define BLOB_H 13
#define SHAPE_COUNT 6

#define BG_COLOR ST77XX_BLACK
#define FG_COLOR ST77XX_WHITE

#define L_MSK_3X3 0b0000000100100100
#define R_MSK_3X3 0b0000000001001001
#define L_MSK_4X4 0b1000100010001000
#define R_MSK_4X4 0b0001000100010001

static const uint16_t colors[] = {
  ST77XX_BLACK, ST77XX_RED, ST77XX_GREEN, ST77XX_BLUE,
  ST77XX_YELLOW, ST77XX_ORANGE, ST77XX_MAGENTA
};

#endif