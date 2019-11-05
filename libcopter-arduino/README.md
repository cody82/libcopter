# libcopter for ESP32

# Connections

|  ESP32 Pin | Function | Input |
|---|---|---|
| GPIO36 | ADC1_CH0 | Throttle |
| GPIO39 | ADC1_CH3 | Pitch |
| GPIO34 | ADC1_CH6 | Yaw |
| GPIO35 | ADC1_CH7 | Roll |
| GPIO32 | ADC1_CH4 | NC |
| GPIO33 | ADC1_CH5 | NC |
| GPIO25 | Digital Input, Int. Pull-Down, Active Low | Takeoff |
| GPIO26 | Digital Input, Int. Pull-Down, Active Low | Land |
| GPIO27 | Digital Input, Int. Pull-Down, Active Low | Panic |
| GPIO14 | Digital Input, Int. Pull-Down, Active Low | Calibrate |
| GPIO23 | Digital Output, Active Low | Connection Idicator |
| GPIO19 | Digital Output, Active Low | Connection Idicator |
| GPIO18 | Digital Output, Active Low | Connection Idicator |
| GPIO5  | Digital Output, Active Low | Connection Idicator |

![](images/ESP32-DOIT-DEVKIT-V1-Board-Pinout-30-GPIOs.webp)
