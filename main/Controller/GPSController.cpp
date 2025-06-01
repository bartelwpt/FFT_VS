#include "GPSController.h"

#include <Arduino.h>

static const char *TAG = "GPS";

DeviceData GPSController::data;

#define GPS_RX 26
#define GPS_TX 27

bool GPSController::m_updateReady = false;
HardwareSerial GPSController::m_hwSerial = HardwareSerial(2);
TinyGPSPlus GPSController::m_gps;

void GPSController::init() {
  ESP_LOGI(TAG, "Initializing GPS");
  m_hwSerial.begin(9600, SERIAL_8N1, GPS_RX, GPS_TX);
}

void GPSController::startTask() {
  ESP_LOGI(TAG, "Starting GPS-Task");
  xTaskCreatePinnedToCore(GPSController::task, "GPS", 8000, NULL, 1, NULL, 1);
}

void GPSController::task(void *pvParameters) {
  (void)pvParameters;

  while (true) {
    if (m_hwSerial.available()) {
      m_gps.encode(m_hwSerial.read());
    }

    if (m_gps.location.isUpdated() && m_gps.location.lat() != data.latitude &&
        m_gps.location.lng() != data.longitude) {
      ESP_LOGI(TAG, "New GPS Location detected, indicating update ...");

      data.latitude = m_gps.location.lat();
      data.longitude = m_gps.location.lng();
      data.hour = m_gps.time.hour();
      data.minute = m_gps.time.minute();
      data.second = m_gps.time.second();

      m_updateReady = true;
    }

    vTaskDelay(pdMS_TO_TICKS(10));
  }
}

bool GPSController::updateReady() { return m_updateReady; }
void GPSController::updateDone() { m_updateReady = false; }
