#ifndef SCREEN_H
#define SCREEN_H

#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>
#include "shape.h"

#define TFT_CS 10
#define TFT_RST 7
#define TFT_DC 9

class Screen {
public:
  static void start();
  static void addShapeToFrame(Shape *shape);
  static void addBlobToFrame(Blob &blob);
  static void drawFrame();
  static void updateScore(int score);
  static void updateHighScore(int newHighScore);
  static void updateNextShape(Shape *shape);
  static void printGameOver();
  static void printPause();
  static void reset();

private:
  static void resetFrame(int8_t frame[BLOB_W][BLOB_H]);
  static void printext(char *text, int newNumber, int oldNumber, int Offset);
  static void printBigText(char *msg1, char *msg2);
};

#endif