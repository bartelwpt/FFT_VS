#include "GPSDatabase.h"

#include "esp_log.h"
GPSDatabase::GPSDatabase() : maxDevices(30) {
  mutex = xSemaphoreCreateMutex();
  EventDispatcher::instance().subscribe<GPSDataReceivedEvent>(
      [this](const IEvent& e) {
        auto const& gpsEvent = static_cast<const GPSDataReceivedEvent&>(e);
        this->updateDeviceData(gpsEvent.deviceId, gpsEvent.data);
      });
}

void GPSDatabase::updateDeviceData(const DeviceID& id, const GPSData& data) {
  if (xSemaphoreTake(mutex, portMAX_DELAY) == pdTRUE) {
    if (!deviceDataMap.contains(id)) {
      if (deviceDataMap.size() >= maxDevices) {
        xSemaphoreGive(mutex);
        return;
      }
    }
    deviceDataMap.insert(id, data);  // insert or update
    xSemaphoreGive(mutex);
  }
}

bool GPSDatabase::getDeviceData(const DeviceID& id, GPSData* data) {
  std::optional<GPSData> result;
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