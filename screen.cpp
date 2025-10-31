#include "screen.h"

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

void Screen::start() {
  tft.initR(INITR_BLACKTAB); 
  tft.fillScreen(ST77XX_BLACK);
}

void Screen::setCursor(int16_t x, int16_t y) {
    tft.setCursor(x, y);
}

void Screen::drawSquare(int16_t x, int16_t y, int8_t size, uint16_t color) {
  tft.fillRect(x, y, size, size, color);
  tft.drawRect(x, y, size, size, color);
}

void Screen::drawBackground(uint16_t color) {
  tft.fillScreen(color);
}
