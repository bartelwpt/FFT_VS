#pragma once
#include <painlessMesh.h>

#include "DeviceDataController.h"
#include "GPSController.h"
class MeshController {
 public:
  MeshController(GPSController* gps);
  void init();
  void startTask();
  static void task(void* pvParameters);
  static void receiveCallback(uint32_t from, String& msg);
  static void newConnectionCallback(uint32_t nodeId);
  static void changedConnectionCallback();
  static void nodeTimeAdjustedCallback(int32_t offset);
  static void sendBroadcast(const char* msg);

 private:
  static MeshController* instance;
  painlessMesh m_mesh;
  GPSController* m_gps;
  DeviceDataController m_ddController;
};