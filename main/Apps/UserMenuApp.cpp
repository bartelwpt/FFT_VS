#include "UserMenuApp.h"

#include "Text.h"
#include "esp_log.h"

static const char* TAG = "UserMenuApp";
void UserMenuApp::enter() {
  renderer = &UI::Renderer::instance();
  view = new UI::UserMenuScreen();
  renderer->setRoot(view->root());
  SystemControllers::instance().display.clearDisplay();
}

void UserMenuApp::update() { renderer->render(); }

void UserMenuApp::exit() {
  renderer->clearRoot();
  delete view;
  view = nullptr;
}