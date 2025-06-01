#pragma once
#include <painlessMesh.h>

#include "DeviceDataController.h"
class MeshController {
 public:
  MeshController() = default;
  void init();
  void startTask();
  static void task(void* pvParameters);
  static void receiveCallback(uint32_t from, String& msg);
  static void newConnectionCallback(uint32_t nodeId);
  static void changedConnectionCallback();
  static void nodeTimeAdjustedCallback(int32_t offset);
  static void sendBroadcast(const char* msg);

 private:
  static painlessMesh m_mesh;
  DeviceDataController m_ddController;
};