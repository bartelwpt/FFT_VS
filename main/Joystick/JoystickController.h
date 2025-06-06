#pragma once

#include <Arduino.h>

class JoystickController {
 public:
  static JoystickController& getInstance();

  void init();

  // Accessors
  bool isDownPressed() const;
  bool isUpPressed() const;
  bool isLeftPressed() const;
  bool isRightPressed() const;
  bool isMidPressed() const;

  void clearDownPressed();
  void clearUpPressed();
  void clearLeftPressed();
  void clearRightPressed();
  void clearMidPressed();

  void lock();
  void unlock();
  bool locked() const;

 private:
  JoystickController();
  static JoystickController* s_instance;

  // ISR handlers
  static void IRAM_ATTR onJoystickDown();
  static void IRAM_ATTR onJoystickUp();
  static void IRAM_ATTR onJoystickLeft();
  static void IRAM_ATTR onJoystickRight();
  static void IRAM_ATTR onJoystickMid();

  // Pin state flags (volatile because modified in ISR)
  volatile bool m_down = false;
  volatile bool m_up = false;
  volatile bool m_left = false;
  volatile bool m_right = false;
  volatile bool m_mid = false;

  volatile bool m_locked = false;

  // Disable copying
  JoystickController(const JoystickController&) = delete;
  JoystickController& operator=(const JoystickController&) = delete;
};