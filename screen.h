#ifndef SCREEN_H
#define SCREEN_H

#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>
#include "shape.h"

#define TFT_CS 10
#define TFT_RST 8
#define TFT_DC 9

class Screen {
public:
  static void start();
  static void setCursor(int16_t x, int16_t y);
  static void addShapeToFrame(Shape *shape);
  static void addBlobToFrame(Blob &blob);
  static void drawFrame();
  static void updateScore(int32_t score);

private:
  static void resetFrame(int8_t frame[BLOB_W][BLOB_H]);
};

#endif