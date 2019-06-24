#include <DHT.h>          // DHT 헤더 포함
#include <ESP8266WiFi.h>  // 와이파이 헤더 포함
 
String apiKey = "F5PKUBH5GIJ93EF5";  // Thingspeak apikey
const char* ssid = "Up to the SKY";  // 와이파이 ssid (이름
const char* password = "1402036768"; // 와이파이 비밀번호
 
const char* server = "api.thingspeak.com"; // 서버 이름
#define DHTPIN D3                          // Nodemcu 3번 핀
 
DHT dht(DHTPIN, DHT22); // DHT 22를 사용한다고 선언
WiFiClient client;      // 이건 무슨 함수일까.

void setup() { 
  pinMode(DHTPIN,INPUT);         // DHT 핀 모드를 인풋 모드로 설정
  Serial.begin(115200);          // 시리얼 통신 속도 설정      
  delay(10);                     // 딜레이 10ms
  dht.begin();                   // dht 시작!
  
  WiFi.begin(ssid, password);    // 아까는 ssid가 const char* 아니고 char*이라더니 
                                 // 여기서는 왜 되는거야! 참고로 와이파이 시작하는 함수
 
  Serial.println();              // 줄바꾸기
  Serial.println();
  Serial.print("Connecting to ");// connecting to 출력
  Serial.println(ssid);          // ssid 출력
   
  WiFi.begin(ssid, password);
   
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  
}
 
 
void loop() {   
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  char buf[16];
      String strTemp = dtostrf(t, 4, 1, buf);
      String strHumi = dtostrf(h, 4, 1, buf);
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
 
  if (client.connect(server,80)) {  //   "184.106.153.149" or api.thingspeak.com
    String postStr = apiKey;
           postStr +="&field1=";
           postStr += String(strTemp);
           postStr +="&field2=";
           postStr += String(strHumi);
 
     client.print("POST /update HTTP/1.1\n"); 
     client.print("Host: api.thingspeak.com\n"); 
     client.print("Connection: close\n"); 
     client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n"); 
     client.print("Content-Type: application/x-www-form-urlencoded\n"); 
     client.print("Content-Length: "); 
     client.print(postStr.length()); 
     client.print("\n\n"); 
     client.print(postStr);
           
 
     Serial.print("Temperature: ");
     Serial.print(t);
     Serial.print(" degrees Celcius Humidity: "); 
     Serial.print(h);
     Serial.println("% send to Thingspeak");    
  }
  client.stop();
   
  Serial.println("Waiting...");    
  // time between updates
  delay(120000); // 2 mins
}
