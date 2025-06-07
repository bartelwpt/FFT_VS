#pragma once
#include "IAppState.h"
#include "Renderer.h"
#include "UserMenuScreen.h"
#include "Window.h"
class UserMenuApp : public IAppState {
  UI::UserMenuScreen *view = nullptr;
  UI::Renderer *renderer = nullptr;

 public:
  UserMenuApp() {}
  void enter() override;
  void update() override;
  void exit() override;
};