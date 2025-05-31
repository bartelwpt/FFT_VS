#include <inttypes.h>

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ApplicationController.h>
#include <List.hpp>
#include <SPI.h>
#include <Wire.h>
#include <TinyGPSPlus.h>
#include <painlessMesh.h>


#include <State.h>
#include <StateMachine.h>

#define OLED_RESET      -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define JOYSTICK_MID    16
#define JOYSTICK_RIGHT  4
#define JOYSTICK_LEFT   0
#define JOYSTICK_DOWN   2
#define JOYSTICK_UP     15

#define SCREEN_HEIGHT 64
#define SCREEN_WIDTH 128

#define GPS_RX 26
#define GPS_TX 27

static const char *TAG = "FestivalFriendTracker";

/* TASK DECLARATION */
void AppTask( void *pvParameters );

void receieveCallback(uint32_t from, String &msg);
void newConnectionCallback(uint32_t nodeId);
void changedConnectionCallback();
void nodeTimeAdjustedCallback(int32_t offset);

void buttonTest();
void menuTest();
void networkTest();
void gpsTest();



/* Global Object Declaration */
JoystickController jController;
StateMachine stateMachine;
Menu *menu;
List<MenuItem*> menuItemList;
List<State*> stateList;

painlessMesh mesh;

HardwareSerial gpsSerial(2);
TinyGPSPlus gps;

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
ApplicationController appController(&display, &jController, &stateMachine);

State menuState(stateMachine.nextId(), "Menu", &menuTest);
State buttonTestState(stateMachine.nextId(), "Button Test", &buttonTest);
State networkTestState(stateMachine.nextId(), "Network Test", &networkTest);
State gpsTestState(stateMachine.nextId(), "GPS Test", &gpsTest);

/* CALLBACK FUNCTIONS FOR MENU ITEMS */
void btnTestMenuItemCallback() {
  stateMachine.setCurrentState(buttonTestState.id());
}
void nwTestMenuItemCallback() {
  stateMachine.setCurrentState(networkTestState.id());
}

void gpsTestMenuItemCallback() {
  stateMachine.setCurrentState(gpsTestState.id());
}

MenuItem btnTestMenuItem("Button-Test", &btnTestMenuItemCallback);
MenuItem nwTestMenuItem("Network-Test", &nwTestMenuItemCallback);
MenuItem gpsTestMenuItem("GPS-Test", &gpsTestMenuItemCallback);


/* CALLBACK FUNCTIONS FOR Joystick Interrutps */
void IRAM_ATTR onJoystickMid() {
  jController.setMid(true);
}
void IRAM_ATTR onJoystickLeft() {
  jController.setLeft(true);
}
void IRAM_ATTR onJoystickRight() {
  jController.setRight(true);
}
void IRAM_ATTR onJoystickUp() {
  jController.setUp(true);
}
void IRAM_ATTR onJoystickDown() {
  jController.setDown(true);
}


/* INIT FUNCTIONS FOR GLOBAL OBJECTS */
void initMenu() {
  Serial.println("init menu");
  menuItemList.add(&btnTestMenuItem);
  menuItemList.add(&nwTestMenuItem);
  menuItemList.add(&gpsTestMenuItem);
  menu = new Menu(&display, &jController, &menuItemList);
}

void initStateMachine() {
  Serial.println("init StateMachine");
  stateList.add(&buttonTestState);
  stateList.add(&menuState);
  stateList.add(&gpsTestState);
  stateMachine.addStates(stateList);
  stateMachine.setCurrentState(menuState.id());
}

void initJoystick() {
  pinMode(JOYSTICK_DOWN, INPUT_PULLUP);
  pinMode(JOYSTICK_UP, INPUT_PULLUP);
  pinMode(JOYSTICK_LEFT, INPUT_PULLUP);
  pinMode(JOYSTICK_RIGHT, INPUT_PULLUP);
  pinMode(JOYSTICK_MID, INPUT_PULLUP);

  attachInterrupt(JOYSTICK_DOWN, onJoystickDown, FALLING);
  attachInterrupt(JOYSTICK_UP, onJoystickUp, FALLING);
  attachInterrupt(JOYSTICK_LEFT, onJoystickLeft, FALLING);
  attachInterrupt(JOYSTICK_RIGHT, onJoystickRight, FALLING);
  attachInterrupt(JOYSTICK_MID, onJoystickMid, FALLING);
}

void initMesh() {
  ESP_LOGI(TAG, "Starting mesh network ...");
  mesh.setDebugMsgTypes( ERROR | STARTUP );
  mesh.init("Test-SSID", "1234");
  mesh.onReceive(&receieveCallback);
  mesh.onNewConnection(&newConnectionCallback);
  mesh.onChangedConnections(&changedConnectionCallback);
  mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);

}


void receieveCallback(uint32_t from, String &msg)
{
  // do something
  ESP_LOGI(TAG, "received message from %" PRIu32 ": %s", from, msg.c_str());
}

void newConnectionCallback(uint32_t nodeId)
{ 
    ESP_LOGI(TAG, "New connection, nodeId: %" PRIu32, nodeId);
}

void changedConnectionCallback() {
    ESP_LOGI(TAG, "Changed connections %s",mesh.subConnectionJson().c_str());
}

void nodeTimeAdjustedCallback(int32_t offset) {
    ESP_LOGI(TAG, "Adjusted time %" PRIu32 ". Offset = %"PRIi32, mesh.getNodeTime(), offset);
}



/* CALLBACK FUNCTIONS FOR STATES */
void buttonTest() {
  if (jController.locked()) {
    jController.unlock();
    jController.setMid(false);
  }

  jController.unlock();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(2,32);
  display.clearDisplay();
  display.println("Press the button ...");
  display.display();
  display.clearDisplay();
  display.setCursor(50,32);

  if (jController.mid()) {
    jController.setMid(false);
    jController.lock();
    display.setCursor(2,32);
    display.println("switch back to menu");
    display.display();
    delay(2000);
    stateMachine.setCurrentState(menuState.id());
  }

  if (jController.down()) {
    display.println("DOWN");
    jController.setDown(false); 
    display.display();
    delay(500);
  }

  if (jController.up()) {
    display.println("UP");
    jController.setUp(false);
    display.display();
    delay(500);
  }

  if (jController.left()) {
    display.println("LEFT");
    jController.setLeft(false);
    display.display();
    delay(500);
  }

  if (jController.right()) {
    display.println("RIGHT");
    jController.setRight(false);
    display.display();
    delay(500);
  }
}

void networkTest() {
  if (jController.locked()) {
    jController.unlock();
    jController.setMid(false);
  }

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(32,32);
  display.clearDisplay();
  display.println("NETWORKTEST");
  display.display();


  if (jController.mid()) {
    jController.setMid(false);
    jController.lock();
    display.clearDisplay();
    display.setCursor(2,32);
    display.println("back to menu");
    display.display();
    delay(2000);
    stateMachine.setCurrentState(menuState.id());
  }
}

void menuTest() {
  if (jController.locked()) {
    jController.unlock();
  }
  menu->display();
}

void gpsTest() {
  if (jController.locked()) {
    jController.unlock();
    jController.setMid(false);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(10,10);
  display.print("Latitude: ");
  display.print(gps.location.lat());

  display.setCursor(10, 20);
  display.print("Longitude: ");
  display.print(gps.location.lng());
  display.display();
}

void AppTask( void *pvParameters ) {
  Serial.print("AppTask running on core: "); Serial.println(xPortGetCoreID());
  (void) pvParameters;

  while(true) {
    appController.run();
    vTaskDelay(pdMS_TO_TICKS(10));
  }
}

void GPSTask( void *pvParameters) {
  (void) pvParameters;

  while(true) {
    if(gpsSerial.available())
    {
      gps.encode(gpsSerial.read());
    }

    vTaskDelay(pdMS_TO_TICKS(10));
  }
}

void meshTask( void *pvParameters) {
  (void) pvParameters;
  while(true) {
    mesh.update();
  }
}

 
extern "C" void app_main()
{
    initArduino();
    // Do your own thing
    if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
      Serial.println(F("SSD1306 allocation failed"));
      for(;;);
    }

    initJoystick();
    initMenu();
    initStateMachine();
    initMesh();

    gpsSerial.begin(9600, SERIAL_8N1, GPS_RX, GPS_TX);

    xTaskCreatePinnedToCore(
      AppTask,
      "App",
      8000,
      NULL,
      1,
      NULL,
      1
    );

    xTaskCreatePinnedToCore(
      GPSTask,
      "GPS",
      8000,
      NULL,
      1,
      NULL,
      1
    );

    xTaskCreatePinnedToCore(
      meshTask,
      "mesh",
      8000,
      NULL,
      1,
      NULL,
      1
    );
}