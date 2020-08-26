#include <ESP8266WiFi.h>
#include <Servo.h>
 
#define PIN_SERVO D0
 
const char* ssid = "Up to the SKY";
const char* password = "1402036768";
const int[10] = '0'; // 서보 모터의 값을 저장하기 위한 배열
 
WiFiServer server(8080);
Servo myServo;
  
void setup() {

	Serial.begin(115200);
	myServo.attach(PIN_SERVO);
    myServo.write(90);
    delay(500);
    myServo.detach();
 
	WiFi.mode(WIFI_STA);
	WiFi.begin(ssid, password);
 
	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
		Serial.print(".");
	}
	Serial.println("");
	Serial.print("Connected to ");
	Serial.println(ssid);
	Serial.print("IP address: ");
	Serial.println(WiFi.localIP());
 
	server.begin();
	Serial.println("Server started");
}
 
void loop() {
	WiFiClient client = server.available();
	if(!client) {Serial.println("disconnecting");}
 
	Serial.println("새로운 클라이언트");
	client.setTimeout(2000);
 
	String request = client.readStringUntil('\r');
	Serial.println("request: ");
	Serial.println(request);
 
//	while(client.available()) {
//    Serial.println(client.read());
//    Serial.println("hello");
//	}
 
	if(request.indexOf("/pos=") >= 0) {
    Serial.println("shit");
		int pos1 = request.indexOf('=');
		int pos2 = request.indexOf('H');
		String servoPos = request.substring(pos1+1, pos2-1);
 
		myServo.write(servoPos.toInt());
    myServo.attach(PIN_SERVO);
    Serial.print("ServoPos is ");
		Serial.println(servoPos.toInt()); 
    delay(600);
    myServo.detach();
	}
  client.print("HTTP/1.1 200 OK");
  client.print("Content-Type: text/html\r\n\r\n");
	client.print("<!DOCTYPE HTML>");
	client.print("<html>");
	client.print("<head>");
	client.print("<meta charset=\"UTF-8\">");
	client.print("<script src=\"https://ajax.googleapis.com/ajax/libs/jquery/3.3.1/jquery.min.js\"></script>");
	client.print("</head>");
	client.print("<body>");
	client.print("<h2>Servo Control Webpage</h2>");
	client.print("Servo position : <span id=\"servoPos\">90</span>");
	client.print("<br>");
	client.print("<input type=\"range\" min=\"0\" max=\"180\" id=\"servoSlider\" onchange=\"servoWrite(this.value)\"/>");
	client.print("<script>");
	client.print("var slider = document.getElementById(\"servoSlider\");");
	client.print("var servoPos = document.getElementById(\"servoPos\");");
	client.print("slider.oninput = function() {");
	client.print("slider.value = this.value;");
	client.print("servoPos.innerHTML = this.value;}");
	client.print("\n");
	client.print("$.ajaxSetup({timeout:700});");
	client.print("function servoWrite(pos) {");
	client.print("$.get(\"/pos=\" + pos + \"p\");");
	client.print("{Connection: close};}");
	client.print("</script>");
	client.print("</body>");
	client.print("</html>");
 
	Serial.println("클라이언트 연결 해제");
}
