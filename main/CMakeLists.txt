idf_component_register(
    SRCS
        "main.cpp"
        "zones.cpp"
        "espnow.cpp"
        "controller.cpp"
        "pumps.cpp"

    INCLUDE_DIRS "."

    REQUIRES
        esp_wifi
        nvs_flash
        esp_driver_gpio
)