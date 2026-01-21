#ifndef CONFIG_H
#define CONFIG_H

/*
  The purpose of this file is to hold const values for:
  1. Make the game compatible with other screen types and sizes
  2. Alter key game behaviour
  3. Alter IO pins
*/

#include <Adafruit_ST7735.h>

/* User IO: Buttons and Buzzer */
#define BTN_LEFT 8
#define BTN_RIGHT 9
#define BTN_UP 7
#define BTN_DOWN 10
#define BTN_PAUSE 6
#define BUZZER 5 // You can't use 12 if you're using SPI

/* TFT screen */
#define TFT_CS A5
#define TFT_DC A4
#define TFT_RST A3

/* Game */
#define BLOB_W 8
#define BLOB_H 13
#define TICK_SPEED 56  // In ms.
#define SQR_SIZE 10
#define SQR_GAP 2
#define SQR_TSIZE 12
// Used in Tetris
#define MOVE_COUNT_DOWN 12  // In ticks.
#define ROW_SIZE 4
#define ROW_SQR_SIZE 16
#define L_WALL_MSK 0b1000100010001000
#define R_WALL_MSK_COL3 0b0001000100010001
#define R_WALL_MSK_COL4 0b0010001000100010
// Used in Snakes
#define MOVE_COUNT 5             // In ticks.
#define HIGH_NIB_MSK 0b11110000  // Nib = Nibble = half-byte = 4bits.
#define LOW_NIB_MSK 0b00001111

/* Game menu */
#define GAME_LIST_SIZE 2
#define GAME_MENU_ITEM_SIZE GAME_LIST_SIZE + 2

/* Screen/display */
#define SCR_HEIGHT 160
#define SCR_WIDTH 128
#define SM_PAD 3
#define MD_PAD 6
#define TXT_HEIGHT 12
#define BG_COLOR ST77XX_BLACK
#define FG_COLOR ST77XX_WHITE
// Used for Tetris
#define NXT_SQR_SIZE 5
#define NXT_SQR_GAP 1
#define NXT_SQR_TSIZE 6
#define SHAPE_COUNT 7

/* Colors */
#define BLUE_IDX 3
#define RED_IDX 1
#define BLACK_IDX 0

static const uint16_t colors[] = {
    ST77XX_BLACK, ST77XX_RED, ST77XX_GREEN, ST77XX_BLUE,
    ST77XX_YELLOW, ST77XX_ORANGE, ST77XX_MAGENTA};

using GGrid = uint8_t[BLOB_W][BLOB_H];

#endif