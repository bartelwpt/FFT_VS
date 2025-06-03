#include "GPSTestState.h"

GPSTestState::GPSTestState(Adafruit_SSD1306 *display,
                           JoystickController *joystick, GPSController *gps,
                           StateMachine *stateMachine)
    : m_display(display),
      m_joystick(joystick),
      m_gps(gps),
      m_stateMachine(stateMachine) {}

void GPSTestState::enter() {
  m_display->clearDisplay();
  m_display->println("GPS-Test");
  m_display->display();
  vTaskDelay(pdMS_TO_TICKS(2000));
}

void GPSTestState::update() {
  m_display->clearDisplay();
  m_display->print("Fix: ");
  m_display->print()
}
void GPSTestState::exit() {}