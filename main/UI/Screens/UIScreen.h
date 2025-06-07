// UIScreen.h
#pragma once

#include "Window.h"

namespace UI {

class UIScreen {
 public:
  UIScreen() = default;
  virtual ~UIScreen();

  // Returns the root node of this UI screen
  virtual Window* root() const = 0;

  // Called if the screen needs to refresh or update its content
  virtual void update() {}

 protected:
  // Optionally useful for subclasses
  Window* rootNode = nullptr;
};

}  // namespace UI