// RendererTestView.cpp
#include "RendererTestView.h"

namespace UI {

RendererTestView::RendererTestView() {
  rootNode = new Window(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

  titleLine = new Text(2, 2, 1, SSD1306_WHITE, "        User         ");
  firstLine = new Text(2, 14, 1, SSD1306_WHITE, ">YBA - 0044m 0d12h30m");
  secondLine = new Text(2, 24, 1, SSD1306_WHITE, " JNI + 0123m 0d00h00m");

  rootNode->addChild(titleLine);
  rootNode->addChild(firstLine);
  rootNode->addChild(secondLine);
}

Window* RendererTestView::root() const { return rootNode; }

}  // namespace UI