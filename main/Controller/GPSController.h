#pragma once
#include <TinyGPSPlus.h>

#include "DeviceData.h"
class GPSController {
 public:
  GPSController();
  void init();
  void startTask();
  static bool updateReady();
  static void updateDone();

  DeviceData data;

 private:
  static GPSController* instance;
  static void task(void* pvParameters);
  HardwareSerial m_hwSerial;
  TinyGPSPlus m_gps;
  bool m_updateReady;
};