#include <Menu.h>
#include <Arduino.h>

  Menu::Menu(Adafruit_SSD1306 *display, JoystickController * jController, List<MenuItem*> *menuItems)
  : m_display(display), m_jController(jController), m_menuItems(menuItems), m_headerShown(false)
  {
    m_display->clearDisplay();
    m_display->setCursor(0,0);
    m_display->setTextSize(1);
    m_menuItems->get(0)->select();
    m_selectedIndex = 0;
  }

  void Menu::display() {
    unsigned long start = millis();
    showHeader();
    m_display->setCursor(0, 12);
    m_display->setTextColor(WHITE);

    for (int i = 0; i < m_menuItems->getSize(); i++) {
      auto item = m_menuItems->get(i);
      if (item->selected())
        m_display->print("> ");
      m_display->println(item->name());
    }

    if (m_jController->down()) {
      m_jController->setDown(false); 
      m_jController->lock();
      nextIndex();
    }

    if(m_jController->mid())
    {
      m_jController->setMid(false);      
      m_jController->lock();
      m_menuItems->get(m_selectedIndex)->submit();
    }

    m_display->display(); 

    unsigned long end = millis();
    Serial.print("display took: "); Serial.print(end-start); Serial.println(" ms to draw");
    Serial.print("Therefor we have: "); Serial.print(1000/(end-start)); Serial.println(" FPS"); 
  } 

  void Menu::nextIndex() {
    int numOfItems = m_menuItems->getSize();
    if (m_selectedIndex < numOfItems)
      m_selectedIndex++;  

    if (m_selectedIndex >= numOfItems)
      m_selectedIndex = 0;

    selectIndex();
  }

  void Menu::selectIndex() {
    int size = m_menuItems->getSize();

    for (int i = 0; i < size; i++)
    {
      auto item = m_menuItems->get(i);
      if (!item) 
        break;

      if (i == m_selectedIndex) {
        item->select();
      } else {
        item->deselect();
      } 
    }
    m_jController->unlock();
    clearBody();
  }

  void Menu::showHeader()
  {
    uint16_t width = 0, height = 0;
    int16_t xBound = -1, yBound = -1;
    const char * title = "MENU";
    m_display->getTextBounds(title, 0, 0, &xBound, &yBound, &width, &height);
    m_display->setCursor((128-width)/2, 1);
    m_display->fillRect(0, 0, m_display->width(), 10, WHITE);
    m_display->setTextColor(BLACK);
    m_display->println(title);
    m_display->display();
  }

  void Menu::clearBody() 
  {
    m_display->fillRect(0, 10, m_display->width(), m_display->height() - 10, BLACK);
  }




