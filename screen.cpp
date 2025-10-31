#include "screen.h"

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

void Screen::start() {
  tft.initR(INITR_BLACKTAB); 
  tft.setRotation(2);
  delay(200);
  tft.fillScreen(ST77XX_BLACK);
}

void Screen::setCursor(int16_t x, int16_t y) {
    tft.setCursor(x, y);
}

void Screen::drawSquare(int16_t x, int16_t y, int8_t size, uint16_t color) {
  tft.fillRect(x, y, size, size, color);
  // tft.drawRect(x, y, size, size, color);
}

void Screen::drawBackground(uint16_t color) {
  tft.fillScreen(color);
  tft.drawRect(98, 0, 30, 160, ST77XX_WHITE);

  // int x = 0;
  // int y = 120;
  
  // for(int i = 0; i < 8; i++) {
  //   tft.drawRect(x + 2, y, 10, 10, ST77XX_WHITE);
  //   x += 10 + 2;
  // }

  // x = 110;
  // y = 0;

  // for(int i = 0; i < 14; i++) {
  //   tft.drawRect(x, y+1, 10, 10, ST77XX_WHITE);
  //   y += 10 + 1;
  // }
}
