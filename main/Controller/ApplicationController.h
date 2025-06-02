#pragma once
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <MenuController.h>
#include <StateMachine.h>

#include "DeviceDataController.h"
#include "GPSController.h"
#include "JoystickController.h"
#define SCREEN_ADDRESS 0x3C

class ApplicationController {
 public:
  ApplicationController(Adafruit_SSD1306 *display, JoystickController *joystick,
                        GPSController *gps, MenuController *menu,
                        StateMachine *stateMachine);
  void run();
  static void task(void *pvParameters);
  void startTask();
  static void buttonTest();
  static void networkTest();
  static void menuTest();
  static void gpsTest();

 private:
  static ApplicationController *instance;
  Adafruit_SSD1306 *m_display;
  JoystickController *m_jController;
  DeviceDataController *m_ddController{new DeviceDataController()};
  GPSController *m_gpsController;
  MenuController *m_menu;
  StateMachine *m_stateMachine;
  bool m_started{false};
};
