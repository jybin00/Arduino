// AT 커멘드로 제어 안하고 wifi 접속

#include "ESP8266WiFi.h" // wifi 라이브러리 호출

const char* ssid = "Up to the SKY"; // 우리집 와이파이 이름
const char* password = "1402036768"; // 우리집 와이파이 비밀 번호

void setup() {
  Serial.begin(115200); // 시리얼 통신 시작, 통신 속도 되게 빠르네?

  WiFi.begin(ssid,password); // WiFi 시작 초기화가 되면 다음 단계로 넘어감.

  Serial.println();
  Serial.print("Connecting"); // WiFi에 연결중 "Connecting" 출력

  while(WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print("."); // 연결될 때까지 .출력
  }

  Serial.println("Success!"); // 연결이 완료되면 Success 출력
  Serial.print("IP Address is: ");
  Serial.println(WiFi.localIP()); // 연결된 와이파이 IP출력

}

void loop() {
  // put your main code here, to run repeatedly:

}
