#pragma once
#include "IAppState.h"
#include "List.hpp"
#include "SystemControllers.h"
class MenuApp : public IAppState {
 public:
  MenuApp();

  void enter() override;
  void update() override;
  void exit() override;

  void nextIndex();
  void selectIndex();

 private:
  void showHeader();
  void clearBody();
  Adafruit_SSD1306 m_display;
  JoystickController& m_joystick;
  StateMachine m_stateMachine;
  MenuController m_menu;
  List<MenuItem*> m_menuItems;
  GPSDatabase* m_db;

  int m_selectedIndex;
  int m_x0, m_yo, m_width, m_height;
  bool m_headerShown;
};