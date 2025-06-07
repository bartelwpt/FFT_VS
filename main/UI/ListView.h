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

    std::string title = "UserMenu";
    auto* titleLine =
        new Text(SCREEN_WIDTH / 2 - 20, y + 2, 1, SSD1306_WHITE, title.c_str());

    addChild(titleLine);
    titleLine->markDirty();
  }

  void setItems(const std::vector<std::string>& items) {
    data = items;

    int yOffset = 12;
    size_t i = 0;
    for (; i < data.size() && yOffset + lineHeight <= height; ++i) {
      if (i < children.size()) {
        // Update existing child if text differs
        Text* txt = static_cast<Text*>(children[i]);
        if (txt->text() != data[i]) {
          txt->setText(data[i].c_str());
          txt->markDirty();
        }
      } else {
        // Add new child
        auto* text =
            new Text(x + 2, y + yOffset, 1, SSD1306_WHITE, data[i].c_str());
        addChild(text);
        text->markDirty();
      }
      yOffset += lineHeight;
    }

    // Delete any extra children
    while (children.size() > i) {
      delete children.back();
      children.pop_back();
    }
  }

  void draw() override {}

 private:
  std::vector<std::string> data;
  int lineHeight;
};

}  // namespace UI