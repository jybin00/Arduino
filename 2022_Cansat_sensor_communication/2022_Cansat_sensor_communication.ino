#include <SPI.h>
#include <Wire.h>

#include <RF24.h>
RF24 radio(49, 48); // SPI통신을 위한 (CE, CSN) 핀 선언

const byte address[6] = "99074"; // 송신기와 수신기가 동일한 값으로 주소 설정함(5자리)

typedef struct CANSAT_Info
{
  float altitude, longitude;
  short height;
  short Roll, Pitch, Yaw;
  short humidity, temperature;
}Info;

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

static const uint32_t GPSBaud = 9600;

// The TinyGPSPlus object
TinyGPSPlus gps;

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BMP3XX bmp;

void setup() {
  Serial.begin(115200);
  Wire.begin();
  while (!Serial);
  Serial.println(F("2022_Cansat_sensor_communication test"));

  Serial.println(F("RF module setting...."));
  radio.begin();
  radio.openWritingPipe(address); // 데이터를 보낼 수신 주소를 설정
  radio.setDataRate(RF24_250KBPS);
  radio.setChannel(14);
  radio.setPALevel(RF24_PA_MIN); // 송신거리에 따른, 전원공급 파워레벨 설정

  radio.stopListening();  // 모듈을 송신기로 설정
  Serial.println(F("RF module setting end!"));
  
  /*
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  Serial.println(F("Oled found."));
  */

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

  mpu.setFilterIterations(10);
}

void loop() {
  //display.clearDisplay();
  Info info;
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
      humi = dht.readHumidity();
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
