#include "MenuController.h"

#include <Arduino.h>

#include "SystemControllers.h"
// Static variables initialization
static const char* TAG = "MenuController";
MenuController* MenuController::s_instance = nullptr;

MenuController::MenuController() {
  btnTestMenuItem = nullptr;
  nwTestMenuItem = nullptr;
  gpsTestMenuItem = nullptr;

  s_instance = this;
}

MenuController::~MenuController() {
  // MenuItems are allocated dynamically here, so delete if allocated
  delete btnTestMenuItem;
  delete nwTestMenuItem;
  delete gpsTestMenuItem;
}

void MenuController::init() {
  m_display = &SystemControllers::instance().display;
  m_joystick = &SystemControllers::instance().joystick;
  m_app = &SystemControllers::instance().app;

  Serial.println("MenuController: init");

  // Create MenuItems with callback lambdas or static callbacks
  // btnTestMenuItem = new MenuItem("Button-Test", &btnTestMenuItemCallback);
  // nwTestMenuItem = new MenuItem("Network-Test", &nwTestMenuItemCallback);
  gpsTestMenuItem = new MenuItem("GPS-Test", &gpsTestMenuItemCallback);

  // m_menuItemList.add(btnTestMenuItem);
  // m_menuItemList.add(nwTestMenuItem);
  m_menuItemList.add(gpsTestMenuItem);

  m_app->switchToApp(AppID::MENU);
}

List<MenuItem*>& MenuController::getMenuItemList() { return m_menuItemList; }

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
    ESP_LOGI(TAG, "switching to gps app...");
    s_instance->m_app->switchToApp(AppID::GPS);
  }
}