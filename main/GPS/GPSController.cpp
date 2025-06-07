#include "GPSController.h"

#include <Arduino.h>

#include "GPSDataReceivedEvent.h"
#include "SystemControllers.h"
static const char *TAG = "GPS";
GPSController *GPSController::instance = nullptr;

static constexpr int GPS_RX = 26;
static constexpr int GPS_TX = 27;

GPSController::GPSController() : m_hwSerial{2} { instance = this; }

void GPSController::init() {
  ESP_LOGI(TAG, "Initializing GPS");
  instance->m_hwSerial.begin(9600, SERIAL_8N1, GPS_RX, GPS_TX);
}

void GPSController::startTask() {
  ESP_LOGI(TAG, "Starting GPS-Task");
  xTaskCreatePinnedToCore(GPSController::task, "GPS", 8000, NULL, 1, NULL, 1);
}

void GPSController::task(void *pvParameters) {
  (void)pvParameters;

  auto &hwSerial = instance->m_hwSerial;
  auto &gps = instance->m_gps;

  while (true) {
    readFromSerial(hwSerial, gps);
    processGPSData(gps);
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}
void GPSController::readFromSerial(HardwareSerial &serial, TinyGPSPlus &gps) {
  while (serial.available()) {
    gps.encode(serial.read());
  }
}
void GPSController::processGPSData(TinyGPSPlus &gps) {
  if (!gps.location.isValid() || gps.location.age() > 2000) {
    return;
  }

  GPSDataReceivedEvent event{SystemControllers::instance().mesh.deviceId(),
                             instance->bundleData()};

  ESP_LOGI(TAG, "dispatching gps event");
  EventDispatcher::instance().dispatch(event);
}

const GPSData &GPSController::bundleData() {
  cachedData =
      GPSData(m_gps.location.lng(), m_gps.location.lat(), m_gps.time.hour(),
              m_gps.time.minute(), m_gps.time.second(), true);
  return cachedData;
}

bool GPSController::updateReady() { return instance->m_updateReady; }
void GPSController::updateDone() { instance->m_updateReady = false; }
