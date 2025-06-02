// SystemControllers.h
#pragma once
#include <Adafruit_SSD1306.h>

#include "ApplicationController.h"
#include "GPSController.h"
#include "MenuController.h"
#include "MeshController.h"
#include "State.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define SCREEN_ADDRESS 0x3C

// This struct holds references to all major system components
struct SystemControllers {
  Adafruit_SSD1306 display;
  MenuController menu;
  GPSController gps;
  MeshController mesh;
  ApplicationController app;
  JoystickController joystick;
  StateMachine stateMachine;

  // Initialize member references and dependencies
  SystemControllers()
      : display(SCREEN_WIDTH, SCREEN_HEIGHT),
        menu(&display, &joystick, &stateMachine),
        gps(),
        mesh(&gps),
        app(&display, &joystick, &gps, &menu, &stateMachine) {}
};
