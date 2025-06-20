#pragma once
#include <TinyGPSPlus.h>

#include "GPSData.h"
class GPSController {
 public:
  GPSController();
  void init();
  void startTask();
  static bool updateReady();
  static void updateDone();
  static void readFromSerial(HardwareSerial& serial, TinyGPSPlus& gps);
  static void processGPSData(TinyGPSPlus& gps);
  const GPSData& bundleData();

 private:
  GPSData cachedData;
  static GPSController* instance;
  static void task(void* pvParameters);
  HardwareSerial m_hwSerial;
  TinyGPSPlus m_gps;
  bool m_updateReady;
};