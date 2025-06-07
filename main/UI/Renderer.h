#pragma once
#include "Element.h"

class Adafruit_SSD1306;

namespace UI {
class Renderer {
  Renderer();  // Constructor stays private
  static Renderer* s_instance;
  Element* m_root = nullptr;
  Adafruit_SSD1306* m_display = nullptr;

 public:
  Renderer(const Renderer& obj) = delete;
  static Renderer& instance();

  void init(Adafruit_SSD1306* display);  // NEW

  void setRoot(Element* r);
  Element* root();
  void render();
  void clearRoot();
};
}  // namespace UI