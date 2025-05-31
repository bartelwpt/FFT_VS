#pragma once
class JoystickController {
  
public:
  JoystickController();

  bool mid();
  bool up();
  bool down();
  bool left();
  bool right();
  bool locked();

  void setMid(bool val);
  void setUp(bool val);
  void setDown(bool val);
  void setLeft(bool val);
  void setRight(bool val);
  void lock();
  void unlock();

private:
  void resetPins();

  bool m_mid, m_up, m_down, m_left, m_right;
  bool m_locked;
};