#include "EventDispatcher.h"
#include "GPSData.h"
#include "GPSDataReceivedEvent.h"

class GPSDatabase {
 public:
  using DeviceID = uint32_t;  // or uint32_t, depends on your device addressing

  GPSDatabase() : {
    EventDispatcher::instance().subscribe<GPSDataReceivedEvent>(
    [this](const IEvent& e) {
      auto const& gpsEvent = static_cast<const GpsDataReceivedEvent&>(e);
      ESP_LOGI("GPS-DB", "Updating database for id=%" PRIu32,
               gpsEvent.deviceId);
      this->updateDeviceData(gpsEvent.deviceId, gpsEvent.data);
  }

  // Update or insert GPS data for a device
  void updateDeviceData(const DeviceID& id, const GPSData& data) {
      std::lock_guard<SemaphoreHandle_t> lock(mutex_);

      if (deviceDataMap.find(id) == deviceDataMap.end()) {
        // Insert if we haven't reached max capacity
        if (deviceDataMap.size() >= maxDevices) {
          // Could implement eviction policy or just skip new data
          return;
        }
      }
      deviceDataMap[id] = data;
  }

  // Retrieve GPS data for a device; returns std::nullopt if not found
  std::optional<GPSData> getDeviceData(const DeviceID& id) {
      std::lock_guard<SemaphoreHandle_t> lock(mutex_);
      auto it = deviceDataMap.find(id);
      if (it != deviceDataMap.end()) {
        return it->second;
      }
      return std::nullopt;
  }

  // Optional: return all stored devices and data
  std::unordered_map<DeviceID, GPSData> getAllData() {
      std::lock_guard<SemaphoreHandle_t> lock(mutex_);
      return deviceDataMap;
  }

 private:
  std::unordered_map<DeviceID, GPSData> deviceDataMap;
  size_t maxDevices;
  std::SemaphoreHandle_t mutex_;  // protects deviceDataMap
  };