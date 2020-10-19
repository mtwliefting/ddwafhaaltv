#define HAS_OLED false

#include <WiFi.h>
#include <HTTPClient.h>

#if HAS_OLED
# include <Wire.h>
# include <Adafruit_GFX.h>
# include <Adafruit_SSD1306.h>
#else
# include <TFT_eSPI.h>
# include <SPI.h>
#endif


#if HAS_OLED
Adafruit_SSD1306 disp(128, 64, &Wire, -1);
#else
# define TFT_BL          4  // Display backlight control pin
TFT_eSPI disp = TFT_eSPI(135, 240);
#endif



 
const char* ssid = "KeukenConfessies";
const char* password =  "pastapasta";
 
void setup() {
 
  Serial.begin(115200);
  delay(4000);

#if HAS_OLED
    Wire.begin(22, 21);
    if (!disp.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x64
        Serial.println(F("SSD1306 init failed"));
        for (;;) ; // no display no fun
    }
    disp.setTextColor(WHITE);
#else
    if (TFT_BL > 0) {
        pinMode(TFT_BL, OUTPUT);
        digitalWrite(TFT_BL, HIGH);
    }
    disp.init();
    disp.setRotation(1);
    disp.fillScreen(TFT_BLACK);
    disp.setTextColor(TFT_WHITE, TFT_BLACK);
    disp.setTextDatum(TL_DATUM);
#endif

  disp.print("Connecting to WiFi");
  
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
 
  Serial.println("Connected to the WiFi network");
 
}
 
void loop() {


  
  if ((WiFi.status() == WL_CONNECTED)) { //Check the current connection status
 
    HTTPClient http;
   
   http.begin("http://173.249.43.24:5000/api/v1/visitors/total");
    int httpCode = http.GET();                                        //Make the request
 
    if (httpCode > 0) { //Check for the returning code
 
        String payload = http.getString();
       String srt = payload.substring(16, payload.length() - 3); //-3 for own server //
        Serial.println(srt);

  #if HAS_OLED
    disp.clearDisplay();
#else
    disp.fillScreen(TFT_BLACK);
#endif
        disp.setTextSize(3);
        disp.setCursor(0, 0);
        disp.print("DDWAfhaalLive");
        disp.setTextSize(8);
        disp.setCursor(90, 60);
        
        disp.print(srt);
      }
 
    else {
      Serial.println("Error on HTTP request");
    }
 
    http.end(); //Free the resources
  }
 
  delay(5000);
  
  
 
}
