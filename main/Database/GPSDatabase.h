#include <optional>

#include "EventDispatcher.h"
#include "GPSData.h"
#include "GPSDataReceivedEvent.h"
#include "SimpleMap.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

class GPSDatabase {
 public:
  using DeviceID = uint32_t;  // or uint32_t, depends on your device addressing

  GPSDatabase();

  // Update or insert GPS data for a device
  void updateDeviceData(const DeviceID& id, const GPSData& data);

  // Retrieve GPS data for a device; returns std::nullopt if not found
  std::optional<GPSData> getDeviceData(const DeviceID& id);

  // Optional: return all stored devices and data
  SimpleMap<DeviceID, GPSData> getAllData();

 private:
  SimpleMap<DeviceID, GPSData> deviceDataMap;
  size_t maxDevices;
  SemaphoreHandle_t mutex;
  ;  // protects deviceDataMap
};