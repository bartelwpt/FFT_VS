#include "SystemControllers.h"

static const char *TAG = "MAIN";
// ==== ESP32 Entry Point ====
extern "C" void app_main() {
  ESP_LOGI(TAG, "ENTERED MAIN");
  initArduino();

  ESP_LOGI(TAG, "Creating SystemControllers");
  auto &sc = SystemControllers::instance();

  if (!sc.display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    ESP_LOGI(TAG, "Display Allocation failed, exit ....");
    while (true);
  }

  sc.renderer->init(&sc.display);
  sc.joystick.init();
  sc.menu.init();
  sc.mesh.init();
  sc.gps.init();
  sc.app.init();

  sc.mesh.startTask();
  sc.gps.startTask();
  sc.app.startTask();
}