idf_component_register(
    SRCS 
        "main.cpp"
        "StateMachine/StateMachine.cpp"
        "StateMachine/State.cpp"

        "Menu/MenuItem.cpp"
        "Menu/Menu.cpp"

        "UI/Renderer.cpp"

        "Controller/ApplicationController.cpp"
        "Controller/JoystickController.cpp" 
        "Controller/DeviceDataController.cpp"
        "Controller/GPSController.cpp"
        "Controller/MeshController.cpp"
    INCLUDE_DIRS 
        "Controller"
        "Menu"
        "StateMachine"
        "UI"
        "Util"
    REQUIRES
        arduino-esp32 
        List 
        Adafruit_SSD1306
        TinyGPSPlus
        painlessMesh
        json
)