#include "Renderer.h"

#include "SystemControllers.h"
#include "esp_log.h"
using namespace UI;

static const char* TAG = "Renderer";

Renderer* Renderer::s_instance = nullptr;
Renderer::Renderer() { s_instance = this; }

void Renderer::init(Adafruit_SSD1306* display) { m_display = display; }
void Renderer::setRoot(Element* r) { m_root = r; }

Element* Renderer::root() { return m_root; }

void Renderer::render() {
  if (!m_root || !m_display) {
    ESP_LOGI(TAG, "m_root or m_display not initialized, interrupt rendering");
    return;
  }

  m_root->update();
  m_display->display();
}

Renderer& Renderer::instance() {
  static Renderer instance;
  return instance;
}

void Renderer::clearRoot() { m_root = nullptr; }