#include "GPSController.h"

#include <Arduino.h>

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

  auto hwSerial = instance->m_hwSerial;
  auto gps = instance->m_gps;
  auto data = instance->data;

  while (true) {
    if (hwSerial.available()) {
      gps.encode(hwSerial.read());
    }

    if (gps.location.isUpdated() && gps.location.lat() != data.latitude &&
        gps.location.lng() != data.longitude) {
      ESP_LOGI(TAG, "New GPS Location detected, indicating update ...");

      data.latitude = gps.location.lat();
      data.longitude = gps.location.lng();
      data.hour = gps.time.hour();
      data.minute = gps.time.minute();
      data.second = gps.time.second();

      instance->m_updateReady = true;
    }

    vTaskDelay(pdMS_TO_TICKS(10));
  }
}

bool GPSController::updateReady() { return instance->m_updateReady; }
void GPSController::updateDone() { instance->m_updateReady = false; }
