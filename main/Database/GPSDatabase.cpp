#include "GPSDatabase.h"

#include "DatabaseUpdateEvent.h"
#include "GPSDataReceivedEvent.h"
#include "GPSFixAcquiredEvent.h"
#include "SystemControllers.h"
#include "esp_log.h"
static const char* TAG = "GPSDB";
GPSDatabase::GPSDatabase() : maxDevices(30) {
  mutex = xSemaphoreCreateMutex();
  EventDispatcher::instance().subscribe<GPSDataReceivedEvent>(
      [this](const IEvent& e) {
        auto const& gpsEvent = static_cast<const GPSDataReceivedEvent&>(e);
        this->updateDeviceData(gpsEvent.deviceId, gpsEvent.data);
      });

  EventDispatcher::instance().subscribe<GPSFixAcquiredEvent>(
      [this](const IEvent& e) {
        auto const& gpsEvent = static_cast<const GPSFixAcquiredEvent&>(e);
        uint32_t id = SystemControllers::instance().mesh.deviceId();
        this->updateDeviceData(id, gpsEvent.data);
      });
}

void GPSDatabase::updateDeviceData(const DeviceID& id, const GPSData& data) {
  bool updated = false;
  if (xSemaphoreTake(mutex, portMAX_DELAY) == pdTRUE) {
    if (!deviceDataMap.contains(id)) {
      if (deviceDataMap.size() >= maxDevices) {
        xSemaphoreGive(mutex);
        return;
      }
    }
    ESP_LOGI(TAG, "Inserting for id %" PRIu32, id);
    deviceDataMap.insert(id, data);  // insert or update
    updated = true;
    xSemaphoreGive(mutex);
  }

  if (updated) {
    EventDispatcher::instance().dispatch(DatabaseUpdateEvent());
    ESP_LOGI(TAG, "database has %d entries", deviceDataMap.size());
  }
}

bool GPSDatabase::getDeviceData(const DeviceID& id, GPSData* data) {
  if (xSemaphoreTake(mutex, portMAX_DELAY) == pdTRUE) {
    auto result = deviceDataMap.find(id);
    if (result.has_value()) {
      *data = result.value();
      xSemaphoreGive(mutex);
      return true;
    } else {
      xSemaphoreGive(mutex);
      return false;
    }
  }
  return false;
}

SimpleMap<GPSDatabase::DeviceID, GPSData> GPSDatabase::getAllData() {
  SimpleMap<DeviceID, GPSData> result;
  if (xSemaphoreTake(mutex, portMAX_DELAY) == pdTRUE) {
    result = deviceDataMap;
    xSemaphoreGive(mutex);
  }
  return result;
}