#pragma once
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <JoystickController.h>
#include <Menu.h>
#include <StateMachine.h>

#define SCREEN_ADDRESS 0x3C 

class ApplicationController {
public:
  ApplicationController(Adafruit_SSD1306 *display, JoystickController *jController, StateMachine * stateMachine);
  void run();

private:
  Adafruit_SSD1306 *m_display;
  JoystickController *m_jController;
  Menu *m_menu;
  StateMachine *m_stateMachine;

  bool m_started;
};






  
