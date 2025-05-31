#include <ApplicationController.h>

ApplicationController::ApplicationController(Adafruit_SSD1306 *display, JoystickController *jController, StateMachine * stateMachine) : 
  m_display(display), m_started(false), m_jController(jController), m_stateMachine(stateMachine) {}


void ApplicationController::run() {
  Serial.println("ApplicationController::run");   
  if (!m_stateMachine->running() && !m_started) {
    Serial.println("Starting StateMachine ...");
    m_started = true;
    m_stateMachine->start();
  }
}