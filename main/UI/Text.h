#pragma once
#include "Element.h"
#include "SystemControllers.h"
namespace UI {
class Text : public Element {
  Adafruit_SSD1306& display;
  uint16_t color, pointSize;
  const char* text;

 public:
  Text(int _x, int _y, uint16_t _pSz, uint16_t _c, const char* _t)
      : display(SystemControllers::instance().display) {
    x = _x;
    y = _y;
    pointSize = _pSz;
    color = _c;
    text = _t;
  }

  void draw() override {
    display.setTextSize(pointSize);
    display.setTextColor(color);
    display.setCursor(x, y);
    display.print(text);
    display.display();
  }
};
}  // namespace UI