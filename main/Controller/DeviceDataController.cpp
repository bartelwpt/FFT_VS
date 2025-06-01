#include "DeviceDataController.h"

#include <ctime>
static const char* TAG = "DeviceController";
std::map<uint32_t, DeviceData> DeviceDataController::m_data;
std::map<uint32_t, std::string> DeviceDataController::m_usernames;
DeviceData DeviceDataController::m_deviceData;

DeviceDataController::DeviceDataController() {
  m_usernames.insert(std::pair<uint32_t, std::string>(DEVICE_0, USER_0));
  m_usernames.insert(std::pair<uint32_t, std::string>(DEVICE_1, USER_1));
}

void DeviceDataController::addDeviceData(uint32_t userId,
                                         const DeviceData& data) {
  if ((m_data.contains(userId) && !data.isNewer(m_data[userId])) ||
      userId == 0) {
    // Our existing data is more recent
    return;
  }

  ESP_LOGI(TAG, "New data from %s: lon=%.6f lat=%.6f",
           m_usernames[userId].c_str(), data.longitude, data.latitude);

  m_data[userId] = data;
}

void DeviceDataController::updateData(double lat, double lon, int h, int m,
                                      int s) {
  m_deviceData.latitude = lat;
  m_deviceData.longitude = lon;
  m_deviceData.hour = h;
  m_deviceData.minute = m;
  m_deviceData.second = s;
  addDeviceData(m_deviceId, m_deviceData);
}

void DeviceDataController::setDeviceId(uint32_t id) { m_deviceId = id; }

const char* DeviceDataController::updateMessage() {
  return m_deviceData.serialize();
}