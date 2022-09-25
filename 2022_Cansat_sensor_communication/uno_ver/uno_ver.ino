//////////////////////////******* Libraries ********
#include <SPI.h>
#include <Wire.h>

// BMP390 libraries
#include <Adafruit_Sensor.h>
#include "Adafruit_BMP3XX.h"

// DHT22 libraries
#include "DHT.h"

// GPS libraries
#include "TinyGPSPlus.h"
#include "SoftwareSerial.h"

// MPU9250
#include "MPU9250.h"

#include <RF24.h>
////////////////////////////////////////////////////////

typedef struct CANSAT_Info
{
  float latitude, longitude;
  short altitudee;
  short Roll, Pitch, Yaw;
  short humidity, temperature;
  char test[2] = {'h', 'i'};
}Info;

RF24 radio(7, 8); // SPI통신을 위한 (CE, CSN) 핀 선언
const byte address[6] = "99074"; // 송신기와 수신기가 동일한 값으로 주소 설정함(5자리)

#define DHTPIN 5
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

MPU9250 mpu;

// The TinyGPSPlus object
TinyGPSPlus gps;
SoftwareSerial ss(3,4); // RXPin, TXPin
static const uint32_t GPSBaud = 9600;

#define SEALEVELPRESSURE_HPA (1013.25)
Adafruit_BMP3XX bmp;

bool rf_on_off = true;

void setup() {
  Serial.begin(115200);
  pinMode(6, INPUT);
  pinMode(13, OUTPUT);
  Wire.begin();
  while (!Serial);
  Serial.println(F("2022_Cansat_sensor_communication test"));

  Serial.println(F("RF module setting...."));
  radio.begin();
  radio.openWritingPipe(address); // 데이터를 보낼 수신 주소를 설정
  radio.setDataRate(RF24_250KBPS);
  radio.setChannel(14);
  radio.setPALevel(RF24_PA_LOW); // 송신거리에 따른, 전원공급 파워레벨 설정
  Serial.println(F("RF module setting end!"));
  
  /*
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  Serial.println(F("Oled found."));
  */

  if (!bmp.begin_I2C()) {   // hardware I2C mode, can pass in address & alt Wire
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
  
  ss.begin(GPSBaud);

  Serial.println(TinyGPSPlus::libraryVersion());
  delay(1000);
  Serial.println(F("GPS ready"));

  dht.begin();
  Serial.println(F("dht begin."));

  mpu.setFilterIterations(15);
  mpu.setMagneticDeclination(-8.52);
  mpu.calibrateAccelGyro();
  mpu.calibrateMag();
  radio.stopListening();  // 모듈을 송신기로 설정
}

void loop() {
  Info info;
  bmp.performReading();

  if(rf_on_off == true){  // rf 수신
    if(digitalRead(6) == LOW){
      rf_on_off = false;
      radio.startListening();
      digitalWrite(13,LOW);
      Serial.println("start Listening");
      delay(500);
      }
  }
  else if(rf_on_off == false){   // rf 송신
    if(digitalRead(6) == LOW){
      rf_on_off = true;
      radio.stopListening();
      Serial.println("start Shooting");
      digitalWrite(13,HIGH);
      delay(500);
      }
  }
  
  if (mpu.update()) {
    static uint32_t prev_ms = millis();
    if (millis() > prev_ms + 10) {
        short roll = mpu.getRoll()*100;
        short yaw = mpu.getYaw()*100;
        short pitch = mpu.getPitch()*100;
        info.Roll = roll;
        info.Yaw = yaw;
        info.Pitch = pitch;
        prev_ms = millis();
      }
  }

  if (bmp.temperature) {
    static uint32_t prev_ms = millis();
    if (millis() > prev_ms + 1005) {
      info.temperature = bmp.temperature;
      info.altitudee = bmp.readAltitude(SEALEVELPRESSURE_HPA);
      info.humidity = dht.readHumidity();
      prev_ms = millis();
      }
  }  

  // This sketch displays information every time a new sentence is correctly encoded.
  while (ss.available() > 0){
    if (gps.encode(ss.read())){
      if (gps.location.isValid())
      {
        info.latitude = gps.location.lat();
        info.longitude = gps.location.lng();
      }
    }
  }
  bool report = radio.write(&info, sizeof(Info));
  if(report){
    Serial.println(F("Transmission successful!"));
  }
 
}
