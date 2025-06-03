#pragma once
#include <Arduino.h>

#include <List.hpp>
#include <map>
#include <string>

#include "DeviceIDs.h"
#include "GPSData.h"

class DeviceDataController {
 public:
  DeviceDataController();
  static void addGPSData(uint32_t userId, const GPSData& gpsData);
  static const char* updateMessage();
  void updateData(double lon, double lat, int h, int m, int s, bool f);
  void setDeviceId(uint32_t id);

 private:
  static std::map<uint32_t, GPSData> m_data;
  static std::map<uint32_t, std::string> m_usernames;
  static GPSData m_gpsData;
  uint32_t m_deviceId{0};
};