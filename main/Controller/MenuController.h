#pragma once

#include <Adafruit_SSD1306.h>

#include <List.hpp>

#include "JoystickController.h"
#include "Menu.h"  // Your Menu class header
#include "StateMachine.h"

class MenuController {
 public:
  MenuController(Adafruit_SSD1306* display, JoystickController* joystick,
                 StateMachine* stateMachine);
  ~MenuController();

  void init();
  void run();
  Menu* getMenu() const;

  // States for callbacks to use:
  static State* s_buttonTestState;
  static State* s_networkTestState;
  static State* s_gpsTestState;
  static State* s_menuState;

 private:
  static void btnTestMenuItemCallback();
  static void nwTestMenuItemCallback();
  static void gpsTestMenuItemCallback();

  Adafruit_SSD1306* m_display;
  JoystickController* m_joystick;
  StateMachine* m_stateMachine;

  Menu* m_menu;
  List<MenuItem*> m_menuItemList;

  static MenuController* s_instance;

  // Menu items
  MenuItem* btnTestMenuItem;
  MenuItem* nwTestMenuItem;
  MenuItem* gpsTestMenuItem;
};