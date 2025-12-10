#ifndef SCREEN_H
#define SCREEN_H

#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>
#include "config.h"
#include "game.h"

#define TFT_CS 10
#define TFT_RST 7
#define TFT_DC 9

class Screen {
public:
  static void start(int8_t nextFrame[BLOB_W][BLOB_H]);
  static void drawFrame(int8_t currentFrame[BLOB_W][BLOB_H]);
  static void resetFrame(int8_t frame[BLOB_W][BLOB_H]);
  static void updateScore(int score);
  static void updateHighScore(int newHighScore);
  static void updateNextShape(uint16_t nextShape, int8_t color);
  static void printGameOver();
  static void reset();
  static void printMenuText(GameMenuItem items[GAME_LIST_SIZE + 1]);
  static void updateMenu(GameMenu menu, GameMenuItem items[GAME_LIST_SIZE + 1]);

private:
  static void printLabelNumber(char *label, int newNumber, int oldNumber, int Offset);
  static void printText(char *text, int x, int y, uint16_t color);
  static void printNumber(int number, int x, int y, uint16_t color);
  static void printBigText(char *msg1, char *msg2);
};

#endif