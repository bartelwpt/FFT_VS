#pragma once
#include <Adafruit_SSD1306.h>

class State {
public:
  State(int id, const char * name, void (*callback)());

  void run();
  const char * name();
  int id();
  bool finished();

private: 
  int m_id;
  bool m_finished;
  const char * m_name;
  Adafruit_SSD1306 * m_display;
  void (*m_callback)();
};