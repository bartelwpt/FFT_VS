idf_component_register(
    SRCS 
        "main.cpp"
        
        # Apps
        "Apps/ApplicationController.cpp"
        "Apps/GPSApp.cpp"
        "Apps/MenuApp.cpp"
        "Apps/RendererTest.cpp"
        "Apps/UserMenuApp.cpp"

        # Apps / StateMachine
        "Apps/StateMachine/StateMachine.cpp"

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

        # UI
        "UI/Renderer.cpp"
        "UI/Screens/RendererTestView.cpp"
        "UI/Screens/UIScreen.cpp"
        "UI/Screens/UserMenuScreen.cpp"

    INCLUDE_DIRS 
        "Apps"
        "Apps/StateMachine"
        "Database"
        "GPS"
        "Joystick"
        "Network"
        "Menu"
        "Event"
        "Util"
        "UI"
        "UI/Screens"

    REQUIRES
        arduino-esp32 
        List 
        Adafruit_SSD1306
        TinyGPSPlus
        painlessMesh
        json
)