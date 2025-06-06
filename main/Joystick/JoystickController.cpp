#include "JoystickController.h"

#include "esp_log.h"

#define JOYSTICK_MID 16
#define JOYSTICK_RIGHT 4
#define JOYSTICK_LEFT 0
#define JOYSTICK_DOWN 2
#define JOYSTICK_UP 15

static const char* TAG = "Joystick";

JoystickController* JoystickController::s_instance = nullptr;

JoystickController& JoystickController::getInstance() {
  static JoystickController instance;
  return instance;
}

JoystickController::JoystickController() { s_instance = this; }

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

bool JoystickController::isDownPressed() const { return m_down; }

bool JoystickController::isUpPressed() const { return m_up; }

bool JoystickController::isLeftPressed() const { return m_left; }

bool JoystickController::isRightPressed() const { return m_right; }

bool JoystickController::isMidPressed() const { return m_mid; }

void JoystickController::clearDownPressed() { m_down = false; }

void JoystickController::clearUpPressed() { m_up = false; }

void JoystickController::clearLeftPressed() { m_left = false; }

void JoystickController::clearRightPressed() { m_right = false; }

void JoystickController::clearMidPressed() { m_mid = false; }

void JoystickController::lock() { m_locked = true; }

void JoystickController::unlock() { m_locked = false; }

bool JoystickController::locked() const { return m_locked; }

void IRAM_ATTR JoystickController::onJoystickDown() {
  if (s_instance && !s_instance->m_locked) {
    s_instance->m_down = true;
    ESP_LOGI(TAG, "Joystick DOWN pressed");
  }
}

void IRAM_ATTR JoystickController::onJoystickUp() {
  if (s_instance && !s_instance->m_locked) {
    s_instance->m_up = true;
  }
}

void IRAM_ATTR JoystickController::onJoystickLeft() {
  if (s_instance && !s_instance->m_locked) {
    s_instance->m_left = true;
  }
}

void IRAM_ATTR JoystickController::onJoystickRight() {
  if (s_instance && !s_instance->m_locked) {
    s_instance->m_right = true;
  }
}

void IRAM_ATTR JoystickController::onJoystickMid() {
  if (s_instance && !s_instance->m_locked) {
    s_instance->m_mid = true;
  }
}
