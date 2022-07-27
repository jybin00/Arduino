#include <SPI.h>
#include <Wire.h>

// oled libraries //
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, 4);

// BMP390 libraries
#include <Adafruit_Sensor.h>
#include "Adafruit_BMP3XX.h"

// DHT22 libraries
#include "DHT.h"
#define DHTPIN 2
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// GPS libraries
#include "TinyGPSPlus.h"

// MPU9250
#include "MPU9250.h"
MPU9250 mpu;
uint8_t device_count = 0;
uint8_t addrs[7] = {0};

static const uint32_t GPSBaud = 9600;

float temp, humi, pressure, alti;
float longitude, latitude;
int day1, month1, year1;

// The TinyGPSPlus object
TinyGPSPlus gps;

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BMP3XX bmp;

void setup() {
  Serial.begin(115200);
  Wire.begin();
  Wire.setClock(100000);
  while (!Serial);
  Serial.println(F("2022_Cansat_sensor_communication test"));
  
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  Serial.println(F("Oled found."));

  if (!bmp.begin_I2C()) {   // hardware I2C mode, can pass in address & alt Wire
  //if (! bmp.begin_SPI(BMP_CS)) {  // hardware SPI mode  
  //if (! bmp.begin_SPI(BMP_CS, BMP_SCK, BMP_MISO, BMP_MOSI)) {  // software SPI mode
    Serial.println(F("Could not find a valid BMP3 sensor, check wiring!"));
    while (1);
  }
  Serial.println(F("bmp sensor found."));
  
  // Set up oversampling and filter initialization
  bmp.setTemperatureOversampling(BMP3_OVERSAMPLING_4X);
  bmp.setPressureOversampling(BMP3_OVERSAMPLING_2X);
  bmp.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_3);
  bmp.setOutputDataRate(BMP3_ODR_25_HZ);

  
  if (!mpu.setup(0x68)) {  // change to your own address
      while (1) {
          Serial.println("MPU connection failed. Please check your connection with `connection_check` example.");
          delay(5000);
      }
  }
  Serial.println(F("MPU9250 found."));
  
  Serial3.begin(GPSBaud);

  Serial.println(TinyGPSPlus::libraryVersion());
  delay(1000);
  Serial.println(F("GPS ready"));

  dht.begin();
  Serial.println(F("dht begin."));
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  mpu.setFilterIterations(10);
}

void loop() {
  display.clearDisplay();
  bmp.performReading();
  
  if (mpu.available()) {
    mpu.update();
    static uint32_t prev_ms = millis();
    if (millis() > prev_ms + 20) {
        print_roll_pitch_yaw();
        prev_ms = millis();
      }
  }

  if (bmp.temperature) {
    static uint32_t prev_ms = millis();
    if (millis() > prev_ms + 2005) {
      temp = bmp.temperature;
      pressure = bmp.pressure/100;
      alti = bmp.readAltitude(SEALEVELPRESSURE_HPA);
      //humi = dht.readHumidity();
      prev_ms = millis();
      }
  }  

  
  // This sketch displays information every time a new sentence is correctly encoded.
  while (Serial3.available() > 0){
    if (gps.encode(Serial3.read())){
      displayInfo();
    }
  }

  display.setCursor(0, 0);
  display.print("T:" + String(temp) + "*C"); 
  display.setCursor(64, 0);
  display.println("H:" + String(humi) + "%");
  display.setCursor(0, 10);
  display.println("P:" + String(pressure) + " hPa"); 
  display.setCursor(0, 20);
  display.println("Alti:" + String(alti) + " m");

  display.setCursor(0,30);
  display.print(F("Location: "));
  
  display.print(latitude, 6);
  display.println(F(","));
  display.setCursor(0,40);
  display.print(longitude, 6);
  
  display.setCursor(0,50);
  display.print(gps.date.month());
  display.print(F("/"));
  display.print(gps.date.day());
  display.print(F("/"));
  display.print(gps.date.year());
  
  display.display();
 
}

void displayInfo()
{
  if (gps.location.isValid())
  {
    latitude = gps.location.lat();
    longitude = gps.location.lng();
  }
  if (gps.date.isValid())
  {
    month1 = gps.date.month();
    day1 = gps.date.day();
    year1 = gps.date.year();
  }
}

void print_roll_pitch_yaw() {
    Serial.print("Yaw, Pitch, Roll: ");
    Serial.print(mpu.getYaw(), 2);
    Serial.print(", ");
    Serial.print(mpu.getPitch(), 2);
    Serial.print(", ");
    Serial.println(mpu.getRoll(), 2);
}
