#include "screen.h"

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

void Screen::start() {
  tft.initR(INITR_BLACKTAB); 
}

void Screen::setCursor(int16_t x, int16_t y) {
    tft.setCursor(x, y);
}

void Screen::drawSquare(int16_t x, int16_t y, uint16_t color) {
  tft.fillRect(x, y, 10, 10, color);
  tft.drawRect(x, y, 10, 10, color);
}
