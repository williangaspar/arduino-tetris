#ifndef SCREEN_H
#define SCREEN_H

#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>

#define TFT_CS 10
#define TFT_RST 8
#define TFT_DC 9

class Screen {
public:
  static void start();
  static void setCursor(int16_t x, int16_t y);
  static void drawSquare(int16_t x, int16_t y, int8_t size, uint16_t color);
  static void drawBackground(uint16_t color);
  static const uint16_t colors[6] = {
    ST77XX_RED, ST77XX_GREEN, ST77XX_BLUE, ST77XX_YELLOW, ST77XX_ORANGE, ST77XX_MAGENTA
  };
};

#endif