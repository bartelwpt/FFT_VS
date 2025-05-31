#pragma once
#include <Window.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

class Renderer {
public:
  Renderer(Adafruit_SSD1306 *display, UI::Window *eparent);
  void render();

private:
  void resetElement(UI::Element *e);
  UI::Window *m_parent;
  Adafruit_SSD1306 *m_display;
};
