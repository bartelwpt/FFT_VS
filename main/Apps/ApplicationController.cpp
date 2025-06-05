#include "ApplicationController.h"

#include "AppSwitchEvent.h"
#include "EventDispatcher.h"
#include "GPSApp.h"
#include "SystemControllers.h"

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
  m_apps.insert(AppID::GPS, new GPSApp());
  // TO-DO: add more apps

  auto state = m_apps.find(AppID::GPS);
  if (state.has_value()) {
    m_stateMachine->setCurrentState(state.value());
  }
}

void ApplicationController::switchToApp(AppID id) {
  auto app = m_apps.find(id);
  if (app.has_value()) {
    ESP_LOGI(TAG, "Switching to app...");
    m_stateMachine->setCurrentState(app.value());
  } else {
    ESP_LOGW(TAG, "AppID not found in registered apps!");
  }
}

void ApplicationController::task(void* pvParameters) {
  while (true) {
    instance->run();
    vTaskDelay(pdMS_TO_TICKS(10));
  }
}

void ApplicationController::startTask() {
  ESP_LOGI(TAG, "Starting App-Task");
  xTaskCreatePinnedToCore(ApplicationController::task, "App", 8000, NULL, 1,
                          NULL, 1);
}

void ApplicationController::run() {
  if (!m_stateMachine->running() && !m_started) {
    m_started = true;
    m_stateMachine->start();
  }

  m_stateMachine->update();
}

// /* CALLBACK FUNCTIONS FOR STATES */
// void ApplicationController::buttonTest() {
//   auto joystick = instance->m_jController;
//   auto display = instance->m_display;
//   auto stateMachine = instance->m_stateMachine;

//   if (joystick->locked()) {
//     joystick->unlock();
//     joystick->setMid(false);
//   }

//   joystick->unlock();
//   display->setTextSize(1);
//   display->setTextColor(WHITE);
//   display->setCursor(2, 32);
//   display->clearDisplay();
//   display->println("Press the button ...");
//   display->display();
//   display->clearDisplay();
//   display->setCursor(50, 32);

//   if (joystick->mid()) {
//     joystick->setMid(false);
//     joystick->lock();
//     display->setCursor(2, 32);
//     display->println("switch back to menu");
//     display->display();
//     delay(2000);
//     stateMachine->setCurrentState(AppStates::MENU_STATE);
//   }

//   if (joystick->down()) {
//     display->println("DOWN");
//     joystick->setDown(false);
//     display->display();
//     delay(500);
//   }

//   if (joystick->up()) {
//     display->println("UP");
//     joystick->setUp(false);
//     display->display();
//     delay(500);
//   }

//   if (joystick->left()) {
//     display->println("LEFT");
//     joystick->setLeft(false);
//     display->display();
//     delay(500);
//   }

//   if (joystick->right()) {
//     display->println("RIGHT");
//     joystick->setRight(false);
//     display->display();
//     delay(500);
//   }
// }

// void ApplicationController::networkTest() {
//   auto joystick = instance->m_jController;
//   auto display = instance->m_display;

//   if (joystick->locked()) {
//     joystick->unlock();
//     joystick->setMid(false);
//   }

//   display->clearDisplay();
//   display->setTextSize(1);
//   display->setTextColor(WHITE);
//   display->setCursor(2, 32);
//   display->println("NETWORKTEST");
//   display->display();

//   if (joystick->mid()) {
//     joystick->setMid(false);
//     joystick->lock();
//     ESP_LOGI(TAG, "Send broadcast test message");
//     char buffer[50];
//     sprintf(buffer, "Message #%d", msgCounter++);
//     MeshController::sendBroadcast(buffer);
//     joystick->unlock();
//   }
// }

// void ApplicationController::menuTest() {
//   auto joystick = instance->m_jController;
//   auto menu = instance->m_menu;
//   if (joystick->locked()) {
//     joystick->unlock();
//   }
//   menu->getMenu()->display();
// }

// void ApplicationController::gpsTest() {
//   auto joystick = instance->m_jController;
//   auto display = instance->m_display;
//   auto gps = instance->m_gpsController;
//   if (joystick->locked()) {
//     joystick->unlock();
//     joystick->setMid(false);
//   }

//   display->clearDisplay();
//   display->setTextSize(1);
//   display->setTextColor(WHITE);
//   display->setCursor(10, 10);
//   display->print("Latitude: ");
//   // display->print(gps->data.latitude);

//   display->setCursor(10, 20);
//   display->print("Longitude: ");
//   // display->print(gps->data.longitude);
//   display->display();
// }