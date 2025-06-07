#pragma once
#include "Element.h"
#include "SystemControllers.h"
namespace UI {
class Window : public Element {
  Adafruit_SSD1306& display;

 public:
  Window(int _x, int _y, int _w, int _h)
      : display(SystemControllers::instance().display) {
    x = _x;
    y = _y;
    width = _w;
    height = _h;
  }

  void draw() override { display.drawRect(x, y, width, height, SSD1306_WHITE); }
};
}  // namespace UI