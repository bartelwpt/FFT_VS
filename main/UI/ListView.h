#pragma once

#include <string>
#include <vector>

#include "Element.h"
#include "Text.h"

namespace UI {

class ListView : public Element {
 public:
  ListView(int x, int y, int width, int height, int lineHeight = 10)
      : lineHeight(lineHeight) {
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
  }

  void setItems(const std::vector<std::string>& items) {
    // Clean up old children
    for (auto* child : children) {
      delete child;
    }
    children.clear();

    data = items;

    int yOffset = 0;
    for (const auto& str : data) {
      if (yOffset + lineHeight > height) break;  // Skip if out of bounds

      auto* text = new Text(x + 2, y + yOffset, 1, SSD1306_WHITE, str.c_str());
      addChild(text);
      yOffset += lineHeight;
    }

    markDirty();  // Inform renderer if using dirty system
  }

  void draw() override {
    // Optional: draw border or background
    // Most rendering handled by children (Text elements)
  }

 private:
  std::vector<std::string> data;
  int lineHeight;
};

}  // namespace UI