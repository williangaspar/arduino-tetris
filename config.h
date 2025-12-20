#ifndef CONFIG_H
#define CONFIG_H

/*
  The purpose of this file is to hold const values that when modifield should:
  1. Make the game compatible with other screen types and sizes
  2. Alter key game behaviour
*/

#include <Adafruit_ST7735.h>

#define GAME_LIST_SIZE 2
#define GAME_MENU_ITEM_SIZE GAME_LIST_SIZE + 1

#define TICK_SPEED 50       // In ms.
#define MOVE_COUNT_DOWN 10  // In ticks.

#define ROW_SIZE 4
#define ROW_SQR_SIZE 16

#define SQR_SIZE 10
#define SQR_GAP 2
#define SQR_TSIZE 12

#define NXT_SQR_SIZE 5
#define NXT_SQR_GAP 1
#define NXT_SQR_TSIZE 6

#define BLOB_W 8
#define BLOB_H 13
#define SHAPE_COUNT 7

#define BG_COLOR ST77XX_BLACK
#define FG_COLOR ST77XX_WHITE

#define L_MSK_C1 0b1000100010001000
#define R_MSK_C1 0b0001000100010001
#define R_MSK_C2 0b0010001000100010

#define HIGH_NIB_MSK 0b11110000  // Nib = Nibble = half-byte = 4bits.
#define LOW_NIB_MSK 0b00001111

#define SCR_HEIGHT 160
#define SCR_WIDTH 128
#define SM_PAD 3
#define MD_PAD 6
#define TXT_HEIGHT 12

#define BLUE_IDX  3
#define RED_IDX  1
#define BLACK_IDX 0

using GGrid = uint8_t[BLOB_W][BLOB_H];

static const uint16_t colors[] = {
  ST77XX_BLACK, ST77XX_RED, ST77XX_GREEN, ST77XX_BLUE,
  ST77XX_YELLOW, ST77XX_ORANGE, ST77XX_MAGENTA
};

#endif