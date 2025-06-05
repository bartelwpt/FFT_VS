#include "GPSApp.h"

#include "AppSwitchEvent.h"
#include "EventDispatcher.h"
#include "SystemControllers.h"

static const char *TAG = "GPS-App";

GPSApp::GPSApp()
    : m_display(SystemControllers::instance().display),
      m_joystick(SystemControllers::instance().joystick),
      m_gps(SystemControllers::instance().gps),
      m_stateMachine(SystemControllers::instance().stateMachine),
      m_db(SystemControllers::instance().db) {}

void GPSApp::enter() {
  m_display.clearDisplay();
  m_display.println("GPS-Test");
  m_display.display();
  vTaskDelay(pdMS_TO_TICKS(2000));
}

void GPSApp::update() {
  drawLocation();

  if (m_joystick.mid()) {
    ESP_LOGI(TAG, "Dispatch AppSwitchEvent{AppStates::MENU}");
    EventDispatcher::instance().dispatch(AppSwitchEvent{AppID::MENU});
  }
}
void GPSApp::exit() { ESP_LOGI(TAG, "Leaving GPSApp ..."); }

void GPSApp::drawLocation() {
  auto deviceData =
      m_db.getDeviceData(SystemControllers::instance().mesh.deviceId());

  if (!deviceData.has_value()) {
    ESP_LOGI(TAG, "DeviceData has no value");
    return;
  }

  m_display.clearDisplay();
  m_display.setTextSize(1);
  m_display.setTextColor(WHITE);
  m_display.setCursor(10, 10);
  m_display.print("Latitude: ");
  m_display.print(deviceData.value().latitude);
  m_display.setCursor(10, 20);
  m_display.print("Longitude: ");
  m_display.print(deviceData.value().longitude);
  m_display.display();
}