#include "MenuApp.h"

#include "AppSwitchEvent.h"
#include "EventDispatcher.h"
#include "SystemControllers.h"

static const char* TAG = "MenuApp";
extern volatile bool downTriggered;

MenuApp::MenuApp()
    : m_display(SystemControllers::instance().display),
      m_joystick(JoystickController::getInstance()),
      m_stateMachine(SystemControllers::instance().stateMachine),
      m_menu(SystemControllers::instance().menu),
      m_db(&SystemControllers::db()) {
  m_menuItems = m_menu.getMenuItemList();
}

void MenuApp::enter() {
  m_display.clearDisplay();
  m_display.println("Menu");
  m_display.display();
  m_selectedIndex = 0;
  selectIndex();
}

void MenuApp::update() {
  showHeader();
  m_display.setCursor(0, 12);
  m_display.setTextColor(WHITE);

  for (int i = 0; i < m_menuItems.getSize(); i++) {
    auto item = m_menuItems.get(i);
    if (item->selected()) m_display.print("> ");
    m_display.println(item->name());
  }

  if (m_joystick.isDownPressed()) {
    m_joystick.clearDownPressed();
    nextIndex();
  }

  if (m_joystick.isMidPressed()) {
    m_joystick.clearMidPressed();
    ESP_LOGI(TAG, "calling submit on selected menuItem");
    m_menuItems.get(m_selectedIndex)->submit();
  }

  m_display.display();
}

void MenuApp::exit() { ESP_LOGI(TAG, "Leaving MenuApp ..."); }

void MenuApp::showHeader() {
  uint16_t width = 0, height = 0;
  int16_t xBound = -1, yBound = -1;
  const char* title = "MENU";
  m_display.getTextBounds(title, 0, 0, &xBound, &yBound, &width, &height);
  m_display.setCursor((128 - width) / 2, 1);
  m_display.fillRect(0, 0, m_display.width(), 10, WHITE);
  m_display.setTextColor(BLACK);
  m_display.println(title);
  m_display.display();
}

void MenuApp::nextIndex() {
  int numOfItems = m_menuItems.getSize();
  if (m_selectedIndex < numOfItems) m_selectedIndex++;

  if (m_selectedIndex >= numOfItems) m_selectedIndex = 0;

  selectIndex();
}

void MenuApp::selectIndex() {
  int size = m_menuItems.getSize();

  for (int i = 0; i < size; i++) {
    auto item = m_menuItems.get(i);
    if (!item) break;

    if (i == m_selectedIndex) {
      item->select();
    } else {
      item->deselect();
    }
  }
  m_joystick.unlock();
  clearBody();
}

void MenuApp::clearBody() {
  m_display.fillRect(0, 10, m_display.width(), m_display.height() - 10, BLACK);
}
