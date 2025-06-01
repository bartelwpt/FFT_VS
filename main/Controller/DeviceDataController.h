#pragma once
#include <Arduino.h>

#include <List.hpp>
#include <map>
#include <string>

#include "DeviceData.h"
#include "DeviceIDs.h"

class DeviceDataController {
 public:
  DeviceDataController();
  static void addDeviceData(uint32_t userId, const DeviceData& DeviceData);
  static const char* updateMessage();
  void updateData(double lon, double lat, int h, int m, int s);
  void setDeviceId(uint32_t id);

 private:
  static std::map<uint32_t, DeviceData> m_data;
  static std::map<uint32_t, std::string> m_usernames;
  static DeviceData m_deviceData;
  uint32_t m_deviceId{0};
};