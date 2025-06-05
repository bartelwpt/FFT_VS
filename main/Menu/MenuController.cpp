#include "MenuController.h"

#include <Arduino.h>

#include "SystemControllers.h"
// Static variables initialization
MenuController* MenuController::s_instance = nullptr;

MenuController::MenuController() {
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
  m_display = &SystemControllers::instance().display;
  m_joystick = &SystemControllers::instance().joystick;
  m_app = &SystemControllers::instance().app;

  Serial.println("MenuController: init");

  // Create MenuItems with callback lambdas or static callbacks
  btnTestMenuItem = new MenuItem("Button-Test", &btnTestMenuItemCallback);
  nwTestMenuItem = new MenuItem("Network-Test", &nwTestMenuItemCallback);
  gpsTestMenuItem = new MenuItem("GPS-Test", &gpsTestMenuItemCallback);

  m_menuItemList.add(btnTestMenuItem);
  m_menuItemList.add(nwTestMenuItem);
  m_menuItemList.add(gpsTestMenuItem);

  m_menu = new Menu(m_display, m_joystick, &m_menuItemList);
  m_app->switchToApp(AppID::MENU);
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
    s_instance->m_app->switchToApp(AppID::BUTTON_TEST);
  }
}

void MenuController::nwTestMenuItemCallback() {
  if (s_instance) {
    s_instance->m_app->switchToApp(AppID::NETWORK_TEST);
  }
}

void MenuController::gpsTestMenuItemCallback() {
  if (s_instance) {
    s_instance->m_app->switchToApp(AppID::GPS);
  }
}