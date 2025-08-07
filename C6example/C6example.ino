#include "Display_ST7789.h"
#include "LVGL_Driver.h"
#include "RGB_lamp.h"
#include "ui.h"

#include <ESP32Time.h> // https://github.com/fbiego/ESP32Time
#include <WiFi.h>

String months[] = {
  "January", "February", "March", "April", "May", "June",
  "July", "August", "September", "October", "November", "December"
};

int pos[7]={69,-69,-46, -23,0,23,46};

ESP32Time rtc(0); 
int zone=1;
const char* ntpServer = "pool.ntp.org";          
String ssid = "xxxxx";
String password = "xxxxxx";

int last_second=0;
String dateString="";
int angle=0;

int n=0; 

void connectWifi()
{ 
    // Show which network we're trying to connect to
    String connectMsg = "Connecting to WiFi: \"" + ssid + "\"";
    LCD_ShowStatusMessage(connectMsg.c_str());
    delay(500);
    
    WiFi.begin(ssid, password);
    
    int dots = 0;
    String statusMsg = "Connecting";
    uint16_t animationY = 20 + 12 * 5; // Calculate Y position (5th message line)
    
    unsigned long startTime = millis();
    unsigned long timeout = 60000; // 1 minute timeout
    
    while (WiFi.status() != WL_CONNECTED && (millis() - startTime) < timeout) {
        // Create blinking dots effect
        String dotsStr = "";
        for(int i = 0; i < dots; i++) {
            dotsStr += ".";
        }
        
        String currentMsg = statusMsg + dotsStr;
        LCD_UpdateStatusMessage(currentMsg.c_str(), animationY);
        
        dots = (dots + 1) % 4; // Cycle through 0, 1, 2, 3 dots
        delay(500);
        
        Serial.println("Connecting to WiFi...");
    }
    
    if (WiFi.status() == WL_CONNECTED) {
        LCD_UpdateStatusMessage("WiFi connected!", animationY);
        Serial.println("WiFi connected!");
    } else {
        LCD_UpdateStatusMessage("Give up on WiFi", animationY);
        Serial.println("WiFi connection timeout - proceeding without WiFi");
    }
    delay(500);
}

void setTime()
{
  if (WiFi.status() == WL_CONNECTED) {
    configTime(3600*zone, 0, ntpServer);
    struct tm timeinfo;
    if (getLocalTime(&timeinfo)){
      rtc.setTimeStruct(timeinfo); 
    }
  } else {
    // Set a default time if no WiFi
    rtc.setTime(0, 0, 12, 1, 1, 2024); // 12:00:00, Jan 1, 2024
    Serial.println("No WiFi - using default time");
  }
}


void setup()
{       
  Serial.begin(115200);
  
  // Initialize display and show initial message
  LCD_Init();
  LCD_Clear(COLOR_BLACK);
  Set_Backlight(10);
  LCD_ShowStatusMessage("Display initialized...");
  delay(500);
  
  LCD_ShowStatusMessage("Initializing LVGL...");
  Lvgl_Init();
  delay(500);
  
  LCD_ShowStatusMessage("Setting up RGB LEDs...");
  Set_Color(200, 200, 2);
  delay(500);
  
  LCD_ShowStatusMessage("Initializing UI...");
  ui_init();
  delay(500);
  
  // WiFi connection will show its own detailed status
  connectWifi();
  
  LCD_ShowStatusMessage("Setting time...");
  setTime();
  delay(500);
  
  LCD_ShowStatusMessage("Setup complete!");
  delay(1000);
}

void loop()
{
  lv_task_handler();
  delay(5);

  

     
    if(rtc.getSecond()!=last_second)
      {
        last_second=rtc.getSecond();
        _ui_label_set_property(ui_timeLBL, _UI_LABEL_PROPERTY_TEXT, rtc.getTime().substring(0,5).c_str()); 
        _ui_label_set_property(ui_secLBL, _UI_LABEL_PROPERTY_TEXT, rtc.getTime().substring(6,8).c_str()); 
        _ui_label_set_property(ui_yearLBL, _UI_LABEL_PROPERTY_TEXT, String(rtc.getYear()).c_str()); 
        dateString=months[rtc.getMonth()]+" "+String(rtc.getDay());
        _ui_label_set_property(ui_dateLBL, _UI_LABEL_PROPERTY_TEXT, dateString.c_str()); 
        Serial.println(rtc.getDayofWeek());
         _ui_label_set_property(ui_secHEX, _UI_LABEL_PROPERTY_TEXT, String(n).c_str()); 
        lv_obj_set_pos(ui_dayPNL, 121, pos[rtc.getDayofWeek()]);  

        n++;
        if(n==7200)
        {
        n=0;
        setTime();
        }  
      }

    // Increment the angle
    angle=angle+10;
    if (angle >= 3600) angle = 0; // Reset angle (in LVGL, angle is in tenths of degrees)

    // Set the new angle
    lv_img_set_angle(ui_Image2, angle);
    RGB_Lamp_Loop(3);
}
