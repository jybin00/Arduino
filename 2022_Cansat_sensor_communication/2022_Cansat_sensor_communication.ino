#include <SPI.h>
#include <Wire.h>
// oled libraries
#include "SSD1306Wire.h"
// BMP390 libraries
#include <Adafruit_Sensor.h>
#include "Adafruit_BMP3XX.h"
// GPS libraries
#include "TinyGPSPlus.h"
#include <SoftwareSerial.h>

// The TinyGPSPlus object
TinyGPSPlus gps;

// The serial connection to the GPS device RX, TX
SoftwareSerial ss(13, 15);

// Initialize the OLED display using Wire library
SSD1306Wire  display(0x3c, D3, D5);


#define SEALEVELPRESSURE_HPA (1013.25)
Adafruit_BMP3XX bmp;

void setup() {
  Serial.begin(115200);
  display.init();

  if (!bmp.begin_I2C()) {   // hardware I2C mode, can pass in address & alt Wire
    Serial.println(F("Could not find a valid BMP3 sensor, check wiring!"));
    while (1);
  }

  bmp.setTemperatureOversampling(BMP3_OVERSAMPLING_8X);
  bmp.setPressureOversampling(BMP3_OVERSAMPLING_4X);
  bmp.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_3);
  bmp.setOutputDataRate(BMP3_ODR_50_HZ);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    while(1); // Don't proceed, loop forever
  }
  
  ss.begin(9600);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.clearDisplay();

}

void loop() {
  if (! bmp.performReading()) {
  Serial.println(F("Failed to perform reading :("));
  return;  }
  display.clearDisplay();
  display.setCursor(0,0);
  display.print("Temp : "); display.print(bmp.temperature); display.println(" C");
  display.print("Press = ");display.print(bmp.pressure / 100.0); display.println(" hPa");
  display.print("Alti = "); display.print(bmp.readAltitude(SEALEVELPRESSURE_HPA)); display.println(" m");
  
  display.setCursor(0,40);
  
  
  if(ss.available() > 0){
    if (gps.encode(ss.read())){
      display.println(F("Location: "));
      if (gps.location.isValid()){
        display.println(gps.location.lat(), 2);
        display.println(F(","));
        display.print(gps.location.lng(), 2);
      }
      else
      {
        display.print(F("No signal"));
      } 
    }
  }
  else
    display.println(F("Invalid")); 

  display.display();
  delay(2000);
}
