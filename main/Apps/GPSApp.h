#pragma once
#include "IAppState.h"
#include "SystemControllers.h"
class GPSApp : public IAppState {
 public:
  GPSApp();

  void enter() override;
  void update() override;
  void exit() override;

 private:
  void drawLocation();
  Adafruit_SSD1306 m_display;
  JoystickController m_joystick;
  GPSController m_gps;
  StateMachine m_stateMachine;
  GPSDatabase m_db;
};