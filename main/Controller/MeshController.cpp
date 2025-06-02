#include "MeshController.h"

#include "GPSController.h"

MeshController* MeshController::instance = nullptr;
static const char* TAG = "MESH";

MeshController::MeshController(GPSController* gps) : m_gps(gps) {
  instance = this;
}

void MeshController::init() {
  ESP_LOGI(TAG, "Starting mesh network ...");
  ESP_LOGI(TAG, "ChipId: %" PRIu32, m_mesh.getNodeId());
  m_mesh.setDebugMsgTypes(ERROR | STARTUP);
  m_mesh.init("Test-SSID", "1234567890");
  m_mesh.onReceive(&MeshController::receiveCallback);
  m_mesh.onNewConnection(&MeshController::newConnectionCallback);
  m_mesh.onChangedConnections(&MeshController::changedConnectionCallback);
  m_mesh.onNodeTimeAdjusted(&MeshController::nodeTimeAdjustedCallback);
}

void MeshController::receiveCallback(uint32_t from, String& msg) {
  ESP_LOGI(TAG, "received message from %" PRIu32 ": %s", from, msg.c_str());
  instance->m_ddController.addDeviceData(from,
                                         DeviceData::deserialize(msg.c_str()));
}

void MeshController::newConnectionCallback(uint32_t nodeId) {
  ESP_LOGI(TAG, "New connection, nodeId: %" PRIu32, nodeId);
}

void MeshController::changedConnectionCallback() {
  ESP_LOGI(TAG, "Changed connections %s",
           instance->m_mesh.subConnectionJson().c_str());
}

void MeshController::nodeTimeAdjustedCallback(int32_t offset) {
  ESP_LOGI(TAG, "Adjusted time %" PRIu32 ". Offset = %" PRIi32,
           instance->m_mesh.getNodeTime(), offset);
}

void MeshController::task(void* pvParameters) {
  (void)pvParameters;
  while (true) {
    instance->m_mesh.update();
    ESP_LOGI(TAG, "GPS update ready? %d", GPSController::updateReady());
    if (GPSController::updateReady()) {
      DeviceDataController::addDeviceData(instance->m_mesh.getNodeId(),
                                          instance->m_gps->data);
      instance->m_mesh.sendBroadcast(DeviceDataController::updateMessage());
      GPSController::updateDone();
    }
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}

void MeshController::startTask() {
  ESP_LOGI(TAG, "Starting Mesh-Task");
  xTaskCreatePinnedToCore(MeshController::task, "Mesh", 8000, NULL, 1, NULL, 1);
}

void MeshController::sendBroadcast(const char* msg) {
  ESP_LOGI(TAG, "Sending broadcast msg: %s", msg);
  instance->m_mesh.sendBroadcast(msg);
}