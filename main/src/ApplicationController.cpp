#include <ApplicationController.h>

ApplicationController::ApplicationController(Adafruit_SSD1306 *display,
                                             JoystickController *jController,
                                             StateMachine *stateMachine)
    : m_display(display),
      m_jController(jController),
      m_stateMachine(stateMachine) {}

void ApplicationController::run() {
  Serial.println("ApplicationController::run");
  if (!m_stateMachine->running() && !m_started) {
    Serial.println("Starting StateMachine ...");
    m_started = true;
    m_stateMachine->start();
  }
}

void ApplicationController::receiveMessage(uint32_t id, const char *msg) {
  DeviceData receivedData = DeviceData::deserialize(msg);
  m_ddController->addDeviceData(id, receivedData);
}

void ApplicationController::setDeviceId(uint32_t id) {
  m_ddController->setDeviceId(id);
}

const char *ApplicationController::prepareMessage() {
  return m_ddController->updateMessage();
}

void ApplicationController::updateGpsData(long lon, long lat, int h, int m,
                                          int s) {
  m_ddController->updateData(lon, lat, h, m, s);
}
