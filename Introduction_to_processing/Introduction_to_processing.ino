#include <DHT.h> // dht 라이브러리 선언

#define DHTPIN 7 // DHT 센서 핀 번호 매크로로 지정

DHT dht(DHTPIN,DHT11); // DHT 센서 객체 선언. 객체가 뭐지?
float humi = 0; // 습도 변수
float temp = 0; // 온도 변수

void setup() 
{
  pinMode(DHTPIN,INPUT); // DHT 핀은 인풋 모드로 설정
  dht.begin(); // DHT 센서 값 읽기 시작
  
  Serial.begin(9600); // 시리얼 통신 시작. 속도는 9600
}

void loop() 
{
  humi = dht.readHumidity();    // 습도값 읽기
  temp = dht.readTemperature(); // 온도값 읽기
  Serial.print("현재 온도 : ");
  Serial.println(temp);
  Serial.print("현재 습도 : ");
  Serial.println(humi);

  delay(1000);
}
