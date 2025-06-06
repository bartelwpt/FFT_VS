idf_component_register(
    SRCS 
        "main.cpp"
        
        # Apps
        "Apps/ApplicationController.cpp"
        "Apps/GPSApp.cpp"
        "Apps/MenuApp.cpp"

        # Database
        "Database/GPSDatabase.cpp"

        # GPS Hardware Logic
        "GPS/GPSController.cpp"

        # Joystick Hardware Logic
        "Joystick/JoystickController.cpp"

        # Mesh Hardware Logic
        "Network/MeshController.cpp"

        # Menu System
        "Menu/MenuItem.cpp"
        "Menu/MenuController.cpp"

        # State Machine
        "StateMachine/StateMachine.cpp"
        "StateMachine/State.cpp"

        # UI
        "UI/Renderer.cpp"

    INCLUDE_DIRS 
        "Apps"
        "Database"
        "GPS"
        "Joystick"
        "Network"
        "Menu"
        "StateMachine"
        "Event"
        "Util"
        "UI"

    REQUIRES
        arduino-esp32 
        List 
        Adafruit_SSD1306
        TinyGPSPlus
        painlessMesh
        json
        NeoGPS
)