#pragma once
#include <TinyGPSPlus.h>

#include "DeviceData.h"
class GPSController {
 public:
  GPSController() = default;
  void init();
  void startTask();
  static bool updateReady();
  static void updateDone();

  static DeviceData data;

 private:
  static void task(void* pvParameters);
  static HardwareSerial m_hwSerial;
  static TinyGPSPlus m_gps;
  static bool m_updateReady;
};