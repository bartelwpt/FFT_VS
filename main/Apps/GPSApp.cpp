#include "GPSApp.h"

#include "AppSwitchEvent.h"
#include "EventDispatcher.h"
#include "SystemControllers.h"

static const char *TAG = "GPS-App";

GPSApp::GPSApp()
    : m_display(SystemControllers::instance().display),
      m_joystick(SystemControllers::instance().joystick),
      m_gps(SystemControllers::instance().gps),
      m_stateMachine(SystemControllers::instance().stateMachine) {}

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
  m_display.clearDisplay();
  m_display.setTextSize(1);
  m_display.setTextColor(WHITE);
  m_display.setCursor(10, 10);
  m_display.print("Latitude: ");

  // GET OWN DATA FROM DATABASE
  // m_display.print(m_gps->data.latitude);

  m_display.setCursor(10, 20);
  m_display.print("Longitude: ");

  // GET OWN DATA FROM DATABASE
  // m_display.print(gps->data.longitude);
  m_display.display();
}