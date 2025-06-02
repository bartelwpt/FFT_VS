#include <ApplicationController.h>

#include "DeviceIDs.h"
#include "GPSController.h"
#include "MeshController.h"

static const char *TAG = "APP";
ApplicationController *ApplicationController::instance;
extern MeshController meshController;
extern State menuState;
static int msgCounter = 0;

ApplicationController::ApplicationController(Adafruit_SSD1306 *display,
                                             JoystickController *joystick,
                                             GPSController *gps,
                                             MenuController *menu,
                                             StateMachine *stateMachine)
    : m_display(display),
      m_jController(joystick),
      m_gpsController(gps),
      m_menu(menu),
      m_stateMachine(stateMachine) {
  instance = this;
}

void ApplicationController::task(void *pvParameters) {
  Serial.print("AppTask running on core: ");
  Serial.println(xPortGetCoreID());

  while (true) {
    instance->run();
    vTaskDelay(pdMS_TO_TICKS(10));
  }
}

void ApplicationController::startTask() {
  xTaskCreatePinnedToCore(ApplicationController::task, "App", 8000, NULL, 1,
                          NULL, 1);
}

void ApplicationController::run() {
  Serial.println("ApplicationController::run");
  if (!m_stateMachine->running() && !m_started) {
    Serial.println("Starting StateMachine ...");
    m_started = true;
    m_stateMachine->start();
  }
}
/* CALLBACK FUNCTIONS FOR STATES */
void ApplicationController::buttonTest() {
  auto joystick = instance->m_jController;
  auto display = instance->m_display;
  auto stateMachine = instance->m_stateMachine;

  if (joystick->locked()) {
    joystick->unlock();
    joystick->setMid(false);
  }

  joystick->unlock();
  display->setTextSize(1);
  display->setTextColor(WHITE);
  display->setCursor(2, 32);
  display->clearDisplay();
  display->println("Press the button ...");
  display->display();
  display->clearDisplay();
  display->setCursor(50, 32);

  if (joystick->mid()) {
    joystick->setMid(false);
    joystick->lock();
    display->setCursor(2, 32);
    display->println("switch back to menu");
    display->display();
    delay(2000);
    stateMachine->setCurrentState(AppStates::MENU_STATE);
  }

  if (joystick->down()) {
    display->println("DOWN");
    joystick->setDown(false);
    display->display();
    delay(500);
  }

  if (joystick->up()) {
    display->println("UP");
    joystick->setUp(false);
    display->display();
    delay(500);
  }

  if (joystick->left()) {
    display->println("LEFT");
    joystick->setLeft(false);
    display->display();
    delay(500);
  }

  if (joystick->right()) {
    display->println("RIGHT");
    joystick->setRight(false);
    display->display();
    delay(500);
  }
}

void ApplicationController::networkTest() {
  auto joystick = instance->m_jController;
  auto display = instance->m_display;

  if (joystick->locked()) {
    joystick->unlock();
    joystick->setMid(false);
  }

  display->clearDisplay();
  display->setTextSize(1);
  display->setTextColor(WHITE);
  display->setCursor(2, 32);
  display->println("NETWORKTEST");
  display->display();

  if (joystick->mid()) {
    joystick->setMid(false);
    joystick->lock();
    ESP_LOGI(TAG, "Send broadcast test message");
    char buffer[50];
    sprintf(buffer, "Message #%d", msgCounter++);
    MeshController::sendBroadcast(buffer);
    joystick->unlock();
  }
}

void ApplicationController::menuTest() {
  auto joystick = instance->m_jController;
  auto menu = instance->m_menu;
  if (joystick->locked()) {
    joystick->unlock();
  }
  menu->getMenu()->display();
}

void ApplicationController::gpsTest() {
  auto joystick = instance->m_jController;
  auto display = instance->m_display;
  auto gps = instance->m_gpsController;
  if (joystick->locked()) {
    joystick->unlock();
    joystick->setMid(false);
  }

  display->clearDisplay();
  display->setTextSize(1);
  display->setTextColor(WHITE);
  display->setCursor(10, 10);
  display->print("Latitude: ");
  display->print(gps->data.latitude);

  display->setCursor(10, 20);
  display->print("Longitude: ");
  display->print(gps->data.longitude);
  display->display();
}