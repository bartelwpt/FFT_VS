#include "MenuController.h"

#include <Arduino.h>

#include "ApplicationController.h"

// Static variables initialization
MenuController* MenuController::s_instance = nullptr;

MenuController::MenuController(Adafruit_SSD1306* display,
                               JoystickController* joystick,
                               StateMachine* stateMachine)
    : m_display(display),
      m_joystick(joystick),
      m_stateMachine(stateMachine),
      m_menu(nullptr) {
  btnTestMenuItem = nullptr;
  nwTestMenuItem = nullptr;
  gpsTestMenuItem = nullptr;

  s_instance = this;
}

MenuController::~MenuController() {
  if (m_menu) {
    delete m_menu;
  }
  // MenuItems are allocated dynamically here, so delete if allocated
  delete btnTestMenuItem;
  delete nwTestMenuItem;
  delete gpsTestMenuItem;
}

Menu* MenuController::getMenu() const { return m_menu; }

void MenuController::init() {
  Serial.println("MenuController: init");

  // Create MenuItems with callback lambdas or static callbacks
  btnTestMenuItem = new MenuItem("Button-Test", &btnTestMenuItemCallback);
  nwTestMenuItem = new MenuItem("Network-Test", &nwTestMenuItemCallback);
  gpsTestMenuItem = new MenuItem("GPS-Test", &gpsTestMenuItemCallback);

  m_menuItemList.add(btnTestMenuItem);
  m_menuItemList.add(nwTestMenuItem);
  m_menuItemList.add(gpsTestMenuItem);

  m_menu = new Menu(m_display, m_joystick, &m_menuItemList);
  s_instance->m_stateMachine->setCurrentState(AppStates::MENU_STATE);
}

void MenuController::run() {
  if (m_joystick->locked()) {
    m_joystick->unlock();
  }
  m_menu->display();
}

// --- CALLBACKS ---
void MenuController::btnTestMenuItemCallback() {
  if (s_instance) {
    s_instance->m_stateMachine->setCurrentState(AppStates::BUTTON_TEST_STATE);
  }
}

void MenuController::nwTestMenuItemCallback() {
  if (s_instance) {
    s_instance->m_stateMachine->setCurrentState(AppStates::NETWORK_TEST_STATE);
  }
}

void MenuController::gpsTestMenuItemCallback() {
  if (s_instance) {
    s_instance->m_stateMachine->setCurrentState(AppStates::GPS_TEST_STATE);
  }
}
