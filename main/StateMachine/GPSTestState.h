#pragma once
#include "IAppState.h"
class GPSTestState : public IAppState {
 public:
  GPSTestState(Adafruit_SSD1306* display, JoystickController* joystick,
               GPSController* gps, StateMachine* stateMachine);

  void enter() override;
  void update() override;
  void exit() override;

 private:
  Adafruit_SSD1306* m_display;
  JoystickController* m_joystick;
  GPSController* m_gps;
  StateMachine* m_stateMachine;
}