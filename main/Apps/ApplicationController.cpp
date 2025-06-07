#include "ApplicationController.h"

#include "AppSwitchEvent.h"
#include "EventDispatcher.h"
#include "GPSApp.h"
#include "MenuApp.h"
#include "RendererTest.h"
#include "SystemControllers.h"
#include "UserMenuApp.h"
ApplicationController* ApplicationController::instance = nullptr;
static const char* TAG = "APP";
ApplicationController::ApplicationController() {
  instance = this;
  EventDispatcher::instance().subscribe<AppSwitchEvent>(
      [this](const IEvent& e) {
        auto const& appSwitchEvent = static_cast<const AppSwitchEvent&>(e);
        switchToApp(appSwitchEvent.targetApp);
      });
}

void ApplicationController::init() {
  m_stateMachine = &SystemControllers::instance().stateMachine;
  setupApps();
}

void ApplicationController::setupApps() {
  m_apps.insert(AppID::MENU, new MenuApp());
  m_apps.insert(AppID::GPS, new GPSApp());
  m_apps.insert(AppID::RENDER_TEST, new RendererTest());
  m_apps.insert(AppID::USER_MENU, new UserMenuApp());
  // TO-DO: add more apps

  auto state = m_apps.find(AppID::USER_MENU);
  if (state.has_value()) {
    m_stateMachine->setCurrentState(state.value());
  }
}

void ApplicationController::switchToApp(AppID id) {
  ESP_LOGI(TAG, "switchToApp requested for id %d", static_cast<int>(id));
  auto app = m_apps.find(id);
  if (app.has_value()) {
    ESP_LOGI(TAG, "Switching to app with ID %d", static_cast<int>(id));
    m_stateMachine->setCurrentState(app.value());
  } else {
    ESP_LOGW(TAG, "AppID not found in registered apps!");
  }
}

void ApplicationController::task(void* pvParameters) {
  while (true) {
    auto start = millis();
    instance->run();
    auto end = millis();
    auto diff = end - start;
    // try to stabilize at 30 fps
    long delay = diff > 0 && diff < 33 ? (33 - diff) : 0;
    vTaskDelay(pdMS_TO_TICKS(delay));
  }
}

void ApplicationController::startTask() {
  ESP_LOGI(TAG, "Starting App-Task");
  xTaskCreatePinnedToCore(ApplicationController::task, "App", 12000, NULL, 1,
                          NULL, 1);
}

void ApplicationController::run() {
  if (!m_stateMachine->running() && !m_started) {
    m_started = true;
    m_stateMachine->start();
  }

  m_stateMachine->update();
}
