#include "GPSDatabase.h"

#include "esp_log.h"
static const char* TAG = "DB";
GPSDatabase::GPSDatabase() : maxDevices(30) {
  mutex = xSemaphoreCreateMutex();
  EventDispatcher::instance().subscribe<GPSDataReceivedEvent>(
      [this](const IEvent& e) {
        auto const& gpsEvent = static_cast<const GPSDataReceivedEvent&>(e);
        ESP_LOGI(TAG, "Updating database for id=%" PRIu32, gpsEvent.deviceId);
        this->updateDeviceData(gpsEvent.deviceId, gpsEvent.data);
      });
}

void GPSDatabase::updateDeviceData(const DeviceID& id, const GPSData& data) {
  ESP_LOGI(TAG, "deviceDataMap size: %d, maxDevices: %d",
           static_cast<int>(deviceDataMap.size()),
           static_cast<int>(maxDevices));
  if (xSemaphoreTake(mutex, portMAX_DELAY) == pdTRUE) {
    if (!deviceDataMap.contains(id)) {
      if (deviceDataMap.size() >= maxDevices) {
        xSemaphoreGive(mutex);
        return;
      }
    }
    ESP_LOGI(TAG, "inserting for id %" PRIu32 " : %f, %f", id, data.longitude,
             data.latitude);
    deviceDataMap.insert(id, data);  // insert or update
    xSemaphoreGive(mutex);
  }
}

std::optional<GPSData> GPSDatabase::getDeviceData(const DeviceID& id) {
  std::optional<GPSData> result;
  ESP_LOGI(TAG, "Getting device data for id %d", static_cast<int>(id));
  if (xSemaphoreTake(mutex, portMAX_DELAY) == pdTRUE) {
    ESP_LOGI(TAG, "Got Semaphore, trying to find data ...");
    auto result = deviceDataMap.find(id);
    if (result.has_value())
      ESP_LOGI(TAG, "result: %f, %f", result->latitude, result->longitude);
    else
      ESP_LOGW(TAG, "no data found");
    xSemaphoreGive(mutex);
  }
  return result;
}

SimpleMap<GPSDatabase::DeviceID, GPSData> GPSDatabase::getAllData() {
  SimpleMap<DeviceID, GPSData> result;
  if (xSemaphoreTake(mutex, portMAX_DELAY) == pdTRUE) {
    result = deviceDataMap;
    xSemaphoreGive(mutex);
  }
  return result;
}