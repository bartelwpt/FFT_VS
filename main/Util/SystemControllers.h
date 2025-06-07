#pragma once

#include <Adafruit_SSD1306.h>

#include "ApplicationController.h"
#include "GPSController.h"
#include "GPSDatabase.h"
#include "JoystickController.h"
#include "MenuController.h"
#include "MeshController.h"
#include "Renderer.h"
#include "StateMachine.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define SCREEN_ADDRESS 0x3C

struct SystemControllers {
  Adafruit_SSD1306 display;
  UI::Renderer* renderer;
  JoystickController& joystick;
  GPSController gps;
  MeshController mesh;
  StateMachine stateMachine;
  MenuController menu;
  ApplicationController app;

  // Constructor

  SystemControllers()
      : display(SCREEN_WIDTH, SCREEN_HEIGHT),
        renderer(&UI::Renderer::instance()),
        joystick(JoystickController::getInstance()),
        gps(),
        mesh(),
        stateMachine(),
        menu(),
        app() {
    // do nothing with renderer here
  }

  // Singleton accessor
  static SystemControllers& instance() {
    static SystemControllers instance;
    return instance;
  }

  // Singleton accessor for GPSDatabase
  static GPSDatabase& db() {
    static GPSDatabase instance;
    return instance;
  }

  // Delete copy constructor and assignment
  SystemControllers(const SystemControllers&) = delete;
  void operator=(const SystemControllers&) = delete;
};