#include "GPSApp.h"

#include "AppSwitchEvent.h"
#include "EventDispatcher.h"
#include "SystemControllers.h"

static const char* TAG = "GPS-App";

GPSApp::GPSApp()
    : m_display(SystemControllers::instance().display),
      m_joystick(SystemControllers::instance().joystick),
      m_gps(SystemControllers::instance().gps),
      m_stateMachine(SystemControllers::instance().stateMachine),
      m_db(&SystemControllers::db()) {}

void GPSApp::enter() {
  m_display.clearDisplay();
  m_display.println("GPS-Test");
  m_display.display();

  EventDispatcher::instance().subscribe<GPSDataReceivedEvent>(
      [this](const IEvent& e) {
        auto& gpsEvent = static_cast<const GPSDataReceivedEvent&>(e);
        if (gpsEvent.deviceId ==
                SystemControllers::instance().mesh.deviceId() &&
            !m_hasReceivedOwnGPS) {
          m_hasReceivedOwnGPS = true;
        }
      });
  vTaskDelay(pdMS_TO_TICKS(2000));
}

void GPSApp::update() {
  if (!m_hasReceivedOwnGPS) {
    m_display.clearDisplay();
    m_display.setCursor(10, 10);
    m_display.println("Waiting for GPS...");
    m_display.display();
    return;
  }

  drawLocation();

  if (m_joystick.mid()) {
    ESP_LOGI(TAG, "Dispatch AppSwitchEvent{AppStates::MENU}");
    EventDispatcher::instance().dispatch(AppSwitchEvent{AppID::MENU});
  }
}

void GPSApp::exit() { ESP_LOGI(TAG, "Leaving GPSApp ..."); }

void GPSApp::drawLocation() {
  GPSData data;
  bool dataReceived =
      m_db->getDeviceData(SystemControllers::instance().mesh.deviceId(), &data);

  if (!dataReceived) {
    m_display.clearDisplay();
    m_display.setCursor(10, 10);
    m_display.print("Waiting for GPS...");
    m_display.display();
    ESP_LOGI(TAG, "DeviceData has no value");
    return;
  }

  m_display.clearDisplay();
  m_display.setTextSize(1);
  m_display.setTextColor(WHITE);
  m_display.setCursor(10, 10);
  m_display.print("Latitude: ");
  m_display.print(data.latitude);
  m_display.setCursor(10, 20);
  m_display.print("Longitude: ");
  m_display.print(data.longitude);
  m_display.display();
}