#pragma once
#include <painlessMesh.h>

// TO-DO: REMOVE and send data via new event so dd can subscribe
#include "GPSData.h"
class MeshController {
 public:
  MeshController();
  void init();
  void startTask();
  static void task(void* pvParameters);
  static void receiveCallback(uint32_t from, String& msg);
  static void newConnectionCallback(uint32_t nodeId);
  static void changedConnectionCallback();
  static void nodeTimeAdjustedCallback(int32_t offset);
  static void sendBroadcast(const char* msg);
  uint32_t deviceId();

 private:
  GPSData data;
  bool sendUpdate{false};
  static MeshController* instance;

  painlessMesh m_mesh;
};