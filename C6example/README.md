# ESP32 C6 Clock Project by VolosR with LVGL

This project is a digital clock implementation for the ESP32-C6 development board using LVGL graphics library. The clock syncs time with NTP server and displays a beautiful UI with date, time, and animations.

## Hardware Requirements

- ESP32-C6 Development Board
  - [Waveshare ESP32-C6-LCD-1.47 (28563)](https://www.waveshare.com/esp32-c6-lcd-1.47.htm) (wiki)[https://www.waveshare.com/wiki/ESP32-C6-LCD-1.47]

## Software Requirements

### Required Arduino Libraries

1. LVGL Library (v8.x)
   ```
   Tools -> Manage Libraries -> Search for "lvgl" -> Install "lvgl"
   ```

2. ESP32Time Library
   ```
   Tools -> Manage Libraries -> Search for "ESP32Time" -> Install "ESP32Time by fbiego"
   ```

3. TFT_eSPI Library (for ST7789 display)
   ```
   Tools -> Manage Libraries -> Search for "TFT_eSPI" -> Install "TFT_eSPI"
   ```

### LVGL Configuration Setup

1. Create new `lv_conf.h` file (or copy `lv_conf.examplev.8.4.0.h` to `lv_conf.h`):
   - Go to your Arduino libraries folder (usually in Documents/Arduino/libraries/lvgl)
   - Locate the `lv_conf_template.h` file
   - Copy it to your project folder as `lv_conf.h`

2. Configure `lv_conf.h`:
   ```c
   #if 1 /*Set it to "1" to enable content*/
   ```
   Enable fonts by changing "0" to "1"
   ```c
    #define LV_FONT_MONTSERRAT_10 1
    #define LV_FONT_MONTSERRAT_12 1
    #define LV_FONT_MONTSERRAT_14 1
    #define LV_FONT_MONTSERRAT_18 1
    #define LV_FONT_MONTSERRAT_26 1
    #define LV_FONT_MONTSERRAT_48 1
   ```
3. Copy `lvgl/demos` to `lvgl/src/demos`. 

## Project Setup

1. Clone this repository
   ```shell
   git clone [repository-url]
   cd C6example
   ```

2. Configure WiFi Settings
   - Open `C6example.ino`
   - Locate the WiFi configuration section
   - Replace `xxxxx` with your WiFi SSID
   - Replace `xxxxxx` with your WiFi password

## Compilation and Upload

1. Arduino IDE Setup
   - Install latest Arduino IDE
   - Add ESP32 board support:
     1. Go to File -> Preferences
     2. Add `https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json` to Additional Board Manager URLs
     3. Go to Tools -> Board -> Boards Manager
     4. Search for ESP32 and install

2. Board Configuration
   - Select appropriate board: Tools -> Board -> ESP32 Arduino -> ESP32-C6
   - Select appropriate port: Tools -> Port -> [YOUR_BOARD_PORT]
   - Select upload speed: Tools -> Upload Speed -> 921600
   - Tools -> Upload Speed -> Flash Size: "4MB (32Mb)"
   - Tools -> Upload Speed -> Partition Scheme: "No OTA (2MB APP/2MB FATFS)" 

3. Compile and Upload
   - Click the Upload button (→) in Arduino IDE
   - Wait for compilation and upload to complete

## Project Structure

- `C6example.ino`: Main Arduino sketch file
- `Display_ST7789.cpp/h`: Display driver implementation
- `LVGL_Driver.cpp/h`: LVGL configuration and initialization
- `RGB_lamp.cpp/h`: RGB LED control implementation
- `ui.c/h`: UI implementation files
- `assets/`: Contains images and fonts used in the project

## Features

- Real-time clock synchronized with NTP server
- Beautiful LVGL-based user interface
- Day of week indicator
- RGB lamp effects
- Rotating image animation
- Time zone support

## Troubleshooting

1. If compilation fails:
   - Ensure all required libraries are installed
   - Check if the board support package is properly installed
   - Verify LVGL configuration matches your display

2. If time sync fails:
   - Check WiFi credentials
   - Ensure internet connectivity
   - Verify NTP server is accessible

## Credits

- LVGL for the graphics library
- ESP32Time library by fbiego
- Original project contributors
