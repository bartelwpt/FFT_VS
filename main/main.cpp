#include "SystemControllers.h"

SystemControllers sc;

// ==== ESP32 Entry Point ====
extern "C" void app_main() {
  initArduino();

  if (!sc.display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    while (true);
  }

  sc.joystick.init();
  sc.menu.init();
  sc.mesh.init();
  sc.gps.init();

  sc.mesh.startTask();
  sc.gps.startTask();
  sc.app.startTask();
}