// RendererTestView.h
#pragma once

#include "Text.h"
#include "UIScreen.h"

namespace UI {

class RendererTestView : public UIScreen {
 public:
  RendererTestView();
  ~RendererTestView() override = default;

  Window* root() const override;

 private:
  Text* titleLine;
  Text* firstLine;
  Text* secondLine;
};

}  // namespace UI