#include "MeshController.h"

#include "GPSController.h"
#include "GPSDataReceivedEvent.h"
#include "GPSFixAcquiredEvent.h"
#include "NetworkTopologyChangedEvent.h"

MeshController* MeshController::instance = nullptr;
static const char* TAG = "MESH";

MeshController::MeshController() { instance = this; }

void MeshController::init() {
  ESP_LOGI(TAG, "Starting mesh network ...");
  ESP_LOGI(TAG, "ChipId: %" PRIu32, m_mesh.getNodeId());
  m_mesh.setDebugMsgTypes(ERROR | STARTUP);
  m_mesh.init("Test-SSID", "1234567890");
  m_mesh.onReceive(&MeshController::receiveCallback);
  m_mesh.onNewConnection(&MeshController::newConnectionCallback);
  m_mesh.onChangedConnections(&MeshController::changedConnectionCallback);
  m_mesh.onNodeTimeAdjusted(&MeshController::nodeTimeAdjustedCallback);

  EventDispatcher::instance().subscribe<GPSFixAcquiredEvent>(
      [this](const IEvent& e) {
        const auto& evt = static_cast<const GPSFixAcquiredEvent&>(e);
        data = evt.data;
        sendUpdate = true;
      });

  nodes = m_mesh.getNodeList();
}

void MeshController::receiveCallback(uint32_t from, String& msg) {
  GPSData data = GPSData::deserialize(msg.c_str());
  GPSDataReceivedEvent event(from, data);
  EventDispatcher::instance().dispatch(event);
}

void MeshController::newConnectionCallback(uint32_t nodeId) {
  ESP_LOGI(TAG, "New connection, nodeId: %" PRIu32, nodeId);
}

void MeshController::changedConnectionCallback() {
  ESP_LOGI(TAG, "Changed connections %s",
           instance->m_mesh.subConnectionJson().c_str());
  instance->nodes = instance->m_mesh.getNodeList();
  EventDispatcher::instance().dispatch(NetworkTopologyChangedEvent());
}

void MeshController::nodeTimeAdjustedCallback(int32_t offset) {
  ESP_LOGI(TAG, "Adjusted time %" PRIu32 ". Offset = %" PRIi32,
           instance->m_mesh.getNodeTime(), offset);
}

void MeshController::task(void* pvParameters) {
  (void)pvParameters;
  while (true) {
    instance->m_mesh.update();
    if (instance->sendUpdate) {
      instance->m_mesh.sendBroadcast(instance->data.serialize());
    }
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}

void MeshController::startTask() {
  ESP_LOGI(TAG, "Starting Mesh-Task");
  xTaskCreatePinnedToCore(MeshController::task, "Mesh", 8000, NULL, 1, NULL, 1);
}

void MeshController::sendBroadcast(const char* msg) {
  instance->m_mesh.sendBroadcast(msg);
}

uint32_t MeshController::deviceId() { return m_mesh.getNodeId(); }

bool MeshController::isInNetwork(uint32_t id) const {
  for (auto nodeId : nodes) {
    if (nodeId == id) return true;
  }
  return false;
}