// UIScreen.cpp
#include "UIScreen.h"

namespace UI {

UIScreen::~UIScreen() {
  delete rootNode;  // Assumes Window recursively deletes children
  rootNode = nullptr;
}

}  // namespace UI
