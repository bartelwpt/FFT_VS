#pragma once
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <JoystickController.h>
#include <Menu.h>
#include <StateMachine.h>

#include "DeviceDataController.h"
#define SCREEN_ADDRESS 0x3C

class ApplicationController {
 public:
  ApplicationController(Adafruit_SSD1306 *display,
                        JoystickController *jController,
                        StateMachine *stateMachine);
  void receiveMessage(uint32_t id, const char *msg);
  void run();
  void setDeviceId(uint32_t id);
  void updateGpsData(double lon, double lat, int h, int m, int s);
  const char *prepareMessage();

 private:
  Adafruit_SSD1306 *m_display;
  JoystickController *m_jController;
  StateMachine *m_stateMachine;

  DeviceDataController *m_ddController{new DeviceDataController()};
  bool m_started{false};
};
