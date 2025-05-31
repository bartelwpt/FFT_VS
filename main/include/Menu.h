#pragma once
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <JoystickController.h>
#include <MenuItem.h>
#include <List.hpp>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

class Menu {
  
public:
  Menu(Adafruit_SSD1306 *display, JoystickController * jController, List<MenuItem*> *menuItems); 
  void display();
  void nextIndex();
  void selectIndex(); 

private:
  void showHeader();
  void clearBody();

  Adafruit_SSD1306 *m_display;
  JoystickController *m_jController;
  List<MenuItem*> *m_menuItems;
  int m_selectedIndex;
  int m_x0, m_y0, m_width, m_height;
  bool m_headerShown;
};