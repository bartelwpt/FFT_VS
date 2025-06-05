#include <Arduino.h>
#include <JoystickController.h>

#define JOYSTICK_MID 16
#define JOYSTICK_RIGHT 4
#define JOYSTICK_LEFT 0
#define JOYSTICK_DOWN 2
#define JOYSTICK_UP 15
JoystickController *JoystickController::instance = nullptr;

JoystickController::JoystickController() { instance = this; }

void JoystickController::init() {
  pinMode(JOYSTICK_DOWN, INPUT_PULLUP);
  pinMode(JOYSTICK_UP, INPUT_PULLUP);
  pinMode(JOYSTICK_LEFT, INPUT_PULLUP);
  pinMode(JOYSTICK_RIGHT, INPUT_PULLUP);
  pinMode(JOYSTICK_MID, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(JOYSTICK_DOWN), onJoystickDown,
                  FALLING);
  attachInterrupt(digitalPinToInterrupt(JOYSTICK_UP), onJoystickUp, FALLING);
  attachInterrupt(digitalPinToInterrupt(JOYSTICK_LEFT), onJoystickLeft,
                  FALLING);
  attachInterrupt(digitalPinToInterrupt(JOYSTICK_RIGHT), onJoystickRight,
                  FALLING);
  attachInterrupt(digitalPinToInterrupt(JOYSTICK_MID), onJoystickMid, FALLING);
}

bool JoystickController::mid() { return m_mid; }

bool JoystickController::up() { return m_up; }

bool JoystickController::down() { return m_down; }

bool JoystickController::left() { return m_left; }

bool JoystickController::right() { return m_right; }

void JoystickController::setMid(bool val) {
  if (m_locked) return;
  resetPins();
  m_mid = val;
}

void JoystickController::setUp(bool val) {
  if (m_locked) return;
  resetPins();
  m_up = val;
}

void JoystickController::setDown(bool val) {
  if (m_locked) return;
  resetPins();
  m_down = val;
}

void JoystickController::setLeft(bool val) {
  if (m_locked) return;
  resetPins();
  m_left = val;
}

void JoystickController::setRight(bool val) {
  if (m_locked) return;
  resetPins();
  m_right = val;
}

void JoystickController::lock() { m_locked = true; }

void JoystickController::unlock() { m_locked = false; }

bool JoystickController::locked() { return m_locked; }

void JoystickController::resetPins() {
  m_mid = false;
  m_up = false;
  m_down = false;
  m_left = false;
  m_right = false;
}

void IRAM_ATTR JoystickController::onJoystickMid() {
  if (instance && !instance->m_locked) {
    instance->resetPins();
    instance->m_mid = true;
  }
}

void IRAM_ATTR JoystickController::onJoystickLeft() {
  if (instance && !instance->m_locked) {
    instance->resetPins();
    instance->m_left = true;
  }
}

void IRAM_ATTR JoystickController::onJoystickRight() {
  if (instance && !instance->m_locked) {
    instance->resetPins();
    instance->m_right = true;
  }
}

void IRAM_ATTR JoystickController::onJoystickDown() {
  if (instance && !instance->m_locked) {
    instance->resetPins();
    instance->m_down = true;
  }
}

void IRAM_ATTR JoystickController::onJoystickUp() {
  if (instance && !instance->m_locked) {
    instance->resetPins();
    instance->m_up = true;
  }
}
