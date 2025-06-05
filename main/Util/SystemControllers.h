#pragma once

#include <Adafruit_SSD1306.h>

#include "ApplicationController.h"
#include "GPSController.h"
#include "JoystickController.h"
#include "MenuController.h"
#include "MeshController.h"
#include "StateMachine.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define SCREEN_ADDRESS 0x3C

struct SystemControllers {
  Adafruit_SSD1306 display;
  JoystickController joystick;
  GPSController gps;
  MeshController mesh;
  StateMachine stateMachine;
  MenuController menu;
  ApplicationController app;

  SystemControllers()
      : display(SCREEN_WIDTH, SCREEN_HEIGHT),
        joystick(),
        gps(),
        mesh(),
        stateMachine(),
        menu(),
        app() {}

  static SystemControllers& instance() {
    static SystemControllers instance;
    return instance;
  }

  SystemControllers(const SystemControllers&) = delete;
  void operator=(const SystemControllers&) = delete;
};