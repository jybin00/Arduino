#include <ESP8266WiFi.h> // gkdnlgkdnl

#define PIN_LED_RED D0
#define PIN_LED_BLUE D1
#define PIN_LED_YELLOW D2

const char* ssid = "Up to the SKY";
const char* password = "1402036768";

WiFiServer server(80);

void setup() {
  pinMode(PIN_LED_RED, OUTPUT);
  digitalWrite(PIN_LED_RED, LOW);
  pinMode(PIN_LED_BLUE, OUTPUT);
  digitalWrite(PIN_LED_BLUE, LOW);
  pinMode(PIN_LED_YELLOW, OUTPUT);
  digitalWrite(PIN_LED_YELLOW, LOW);
  
  Serial.begin(115200);
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connecting to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
  server.begin();
  Serial.println("Server started");
}

void loop() {
  WiFiClient client = server.available();
  if(!client) return;
  
  Serial.println("새로운 클라이언트");
  client.setTimeout(5000);
  
  String request = client.readStringUntil('\r');
  Serial.println("request: ");
  Serial.println(request);

  if(request.indexOf("/ledoff_r") != -1) {
    digitalWrite(PIN_LED_RED, LOW);
  }
  else if(request.indexOf("/ledon_r") != -1) {
    digitalWrite(PIN_LED_RED, HIGH);
  }
  else if(request.indexOf("/ledoff_b") != -1) {
    digitalWrite(PIN_LED_BLUE, LOW);
  }
  else if(request.indexOf("/ledon_b") != -1) {
    digitalWrite(PIN_LED_BLUE, HIGH);
  }
  else if(request.indexOf("/ledoff_y") != -1) {
    digitalWrite(PIN_LED_YELLOW, LOW);
  }
  else if(request.indexOf("/ledon_y") != -1) {
    digitalWrite(PIN_LED_YELLOW, HIGH);
  }
  else {
    Serial.println("invalid request");
    digitalWrite(PIN_LED_RED, digitalRead(PIN_LED_RED));
    digitalWrite(PIN_LED_BLUE, digitalRead(PIN_LED_BLUE));
    digitalWrite(PIN_LED_YELLOW, digitalRead(PIN_LED_YELLOW));
  }

  while(client.available()) {
    client.read();
  }

  client.print("HTTP/1.1 200 OK");
  client.print("Content-Type: text/html\r\n\r\n");
  client.print("<!DOCTYPE HTML>");
  client.print("<html>");
  client.print("<head>"); 
  client.print("<meta&nbsp;charset=\"UTF-8\">");
  client.print("<title>LED Control Webpage</title>");
  client.print("</head>");
  client.print("<body>");
  client.print("<h2>LED Control Webpage</h2>");
  client.print("<a href='/ledon_r'><button>LED RED ON</button></a> &emsp;&emsp;&emsp;");
  client.print("<a href='/ledoff_r'><button>LED RED OFF</button></a>  ");
  client.print("<br>");
  client.print("<br>");
  
  client.print("<a href='/ledon_b'><button>LED BLUE ON</button></a> &emsp;&emsp; "); // 버튼 스타일 추
  client.print("<a href='/ledoff_b'><button>LED BLUE OFF</button></a>  ");
  client.print("<br>");
  client.print("<br>");
  
  client.print("<a href='/ledon_y'><button>LED YELLOW ON</button></a> &emsp; ");
  client.print("<a href='/ledoff_y'><button>LED YELLOW OFF</button></a>  ");  // 버튼 스타일 추가
  client.print("<br>");
  
  client.print("<br>");
  client.print("LED Status RED : ");
  client.print((digitalRead(PIN_LED_RED)) ? "ON" : "OFF");
  client.print("<br>");
  client.print("<br>");
  client.print("LED Status BLUE : ");
  client.print((digitalRead(PIN_LED_BLUE)) ? "ON" : "OFF");
  client.print("<br>");
  client.print("<br>");
  client.print("LED Status YELLOW : ");
  client.print((digitalRead(PIN_LED_YELLOW)) ? "ON" : "OFF");
  client.print("</body>");
  client.print("</html>");

  Serial.println("클라이언트 연결 해제");
}

