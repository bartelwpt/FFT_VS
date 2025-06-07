#include "RendererTest.h"

#include "Text.h"
#include "esp_log.h"

void RendererTest::enter() {
  renderer = &UI::Renderer::instance();
  view = new UI::RendererTestView();
  // root = new UI::Window(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

  // auto titleLine =
  //     new UI::Text(2, 2, 1, SSD1306_WHITE, "        User         ");
  // auto firstLine =
  //     new UI::Text(2, 14, 1, SSD1306_WHITE, ">YBA - 0044m 0d12h30m");
  // auto secondLine =
  //     new UI::Text(2, 24, 1, SSD1306_WHITE, " JNI + 0123m 0d00h00m");

  // root->addChild(titleLine);
  // root->addChild(firstLine);
  // root->addChild(secondLine);
  // renderer->setRoot(root);

  renderer->setRoot(view->root());
  SystemControllers::instance().display.clearDisplay();
}

void RendererTest::update() { renderer->render(); }

void RendererTest::exit() {
  renderer->clearRoot();
  delete view;
  view = nullptr;
}