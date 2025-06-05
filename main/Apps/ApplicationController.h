#pragma once

#include <vector>

#include "IAppState.h"
#include "SimpleMap.h"
#include "StateMachine.h"
#include "SystemIDs.h"

class ApplicationController {
 public:
  ApplicationController();
  static ApplicationController *instance;
  void init();
  void switchToApp(AppID id);
  void run();
  void startTask();
  static void task(void *pvParameters);

 private:
  StateMachine *m_stateMachine;

  SimpleMap<AppID, IAppState *> m_apps;
  AppID m_currentAppIndex{static_cast<int>(0)};
  bool m_started = false;

  void setupApps();
};