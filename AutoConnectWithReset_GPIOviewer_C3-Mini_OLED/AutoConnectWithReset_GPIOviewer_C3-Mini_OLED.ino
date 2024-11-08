#include <gpio_viewer.h>
#include <FS.h>          // this needs to be first, or it all crashes and burns...
#include <WiFi.h>
#include <Wire.h>
#include <WiFiManager.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET    -1 // Reset pin # (or -1 if sharing Arduino reset pin)

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
GPIOViewer gpio_viewer;
// bitmap of carenuity_logo image
const unsigned char carenuity_logo [] PROGMEM = {
 //Carenuity logo bitmap array here
};
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Wire.begin();
  Serial.println();
  // Initialize the display
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  display.display(); // Clear the buffer

    // Display bitmap
  display.drawBitmap(0, 0,  carenuity_logo, 128, 64, WHITE);
  display.display();
  
  //WiFiManager
  //Local initialization. Once its business is done, there is no need to keep it around
  WiFiManager wifiManager;

  //exit after config instead of connecting
  wifiManager.setBreakAfterConfig(true);

  //reset settings - for testing
  //wifiManager.resetSettings();


  //tries to connect to last known settings
  //if it does not connect it starts an access point with the specified name
  //here  "AutoConnectAP" with password "password"
  //and goes into a blocking loop awaiting configuration
  if (!wifiManager.autoConnect("GPIOviewer", "password")) {
    Serial.println("failed to connect, we should reset as see if it connects");
    delay(3000);
    ESP.restart();
    delay(5000);
  }

  //if you get here you have connected to the WiFi
  display.println("Web App URL:)");


  Serial.println("local ip");
  Serial.println(WiFi.localIP());
   gpio_viewer.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  // Get the IP address of the ESP32
  IPAddress ip = WiFi.localIP();

  // Display the IP address on the OLED display
  display.clearDisplay();
  display.setTextSize(1);      
  display.setTextColor(WHITE); 
  display.startscrollleft(0x00, 0x0F);
  display.setCursor(0,18);     
  display.println("Web App URL:");
  display.setCursor(0,30);
  display.println("http://") ;
  display.println(ip.toString()+":8080");
  display.display();

  delay(10000); // Update every 10 seconds

}
