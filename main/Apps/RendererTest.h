#pragma once
#include "IAppState.h"
#include "Renderer.h"
#include "RendererTestView.h"
#include "Window.h"
class RendererTest : public IAppState {
  UI::RendererTestView *view = nullptr;
  UI::Renderer *renderer = nullptr;

 public:
  RendererTest() {}
  void enter() override;
  void update() override;
  void exit() override;
};