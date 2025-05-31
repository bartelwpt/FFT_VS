#include "Renderer.h"
#include <Window.h>
#include <Rectangle.h>
#include <Text.h>

Renderer::Renderer(Adafruit_SSD1306 *display, UI::Window *parent) : m_display(display), m_parent(parent) 
{
  Serial.println("Renderer created"); 
}

void Renderer::render() {
  UI::Element * ePtr;
  ePtr = dynamic_cast<UI::Element*>(m_parent);
  // auto renderObject = m_parent;
  while (ePtr != nullptr) { 
    if (auto * castObject = dynamic_cast<UI::Window*>(ePtr); castObject != nullptr) {
      if (castObject->dirty())
      {
        // reset display by drawing a black rectangle
        m_display->fillRect(castObject->x(), castObject->y(), castObject->w(), castObject->h(), static_cast<uint16_t>(UI::Element::Color::MUI_BLACK));
      }
    }

    else if (auto * castObject = dynamic_cast<UI::Rectangle*>(ePtr); castObject != nullptr)
    {

    }
  }  
}


void Renderer::resetElement(UI::Element * e)
{

}