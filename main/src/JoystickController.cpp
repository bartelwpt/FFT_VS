#include <JoystickController.h>
#include <Arduino.h>

JoystickController::JoystickController() {
  resetPins();
  m_locked = false;
}

bool JoystickController::mid() { 
  return m_mid; 
}

bool JoystickController::up() { 
  return m_up; 
}

bool JoystickController::down() { 
  return m_down; 
}

bool JoystickController::left() { 
  return m_left; 
}

bool JoystickController::right() { 
  return m_right; 
}

void JoystickController::setMid(bool val) {
  if( m_locked ) return;     
  resetPins(); 
  m_mid = val; 
}

void JoystickController::setUp(bool val) { 
  if ( m_locked ) return;
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
  m_left= val; 
}

void JoystickController::setRight(bool val) { 
  if (m_locked) return;
  resetPins(); 
  m_right = val; 
}

void JoystickController::lock() {
  m_locked = true;
}

void JoystickController::unlock() {
  m_locked = false;
}

bool JoystickController::locked() {
  return m_locked;
}

void JoystickController::resetPins() {
  m_mid = false;
  m_up = false;
  m_down = false;
  m_left = false;
  m_right = false;
}

