#include<stdlib.h>
#include<DHT.h>
#include<WiFi.h>
#include<ESP8266WiFi.h>

int Green_LED = 7; // 초록색 LED 핀 번호 설정
int DHTpin = 5; // 온습도 센서 핀 번호 설정

float Temp = 0; // 온도 변수 설정
float Humi = 0; // 습도 변수 설정

const char* ssid = "Up to the SKY";   // 우리집 와이파이
const char* password = "1402036768";  // 우리집 와이파이 비밀번호

String apiKey = "F5PKUBH5GIJ93EF5"; // thingspeak api key


void setup() {
  pinMode(Green_LED,OUTPUT); // 초록색 LED on
  Serial.begin(115200); // Serial 통신 시작
  
  WiFi.begin(ssid,password); // 와이파이 시작.
  Serial.println();          // 줄 바꾸기
  Serial.print("Connecting"); // WiFi 연결중에 "Connecting" 출력

  while(WiFi.status() != WL_CONNECTED) // 만약 
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("Success!"); // 연결이 완료되면 Success 출력
  Serial.print("IP Address is: ");
  Serial.println(WiFi.localIP()); // 연결된 와이파이 IP 출력

}

void loop() {
  

}
