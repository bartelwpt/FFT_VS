#pragma once

#include <Adafruit_SSD1306.h>

#include <List.hpp>

#include "ApplicationController.h"
#include "JoystickController.h"
#include "MenuItem.h"
#include "StateMachine.h"
class MenuController {
 public:
  MenuController();
  ~MenuController();

  void init();
  void run();

  List<MenuItem*>& getMenuItemList();

 private:
  static void btnTestMenuItemCallback();
  static void nwTestMenuItemCallback();
  static void gpsTestMenuItemCallback();

  Adafruit_SSD1306* m_display;
  JoystickController* m_joystick;
  ApplicationController* m_app;

  List<MenuItem*> m_menuItemList;

  static MenuController* s_instance;

  // Menu items
  MenuItem* btnTestMenuItem;
  MenuItem* nwTestMenuItem;
  MenuItem* gpsTestMenuItem;
};