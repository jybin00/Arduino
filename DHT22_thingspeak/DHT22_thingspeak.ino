#include <DHT.h>
#include <ESP8266WiFi.h>
 
// replace with your channel's thingspeak API key, 
String apiKey = "F5PKUBH5GIJ93EF5";
const char* ssid = "Up to the SKY";
const char* password = "1402036768";
 
const char* server = "api.thingspeak.com";
#define DHTPIN D3 // D2 pin on Nodemcu
 
DHT dht(DHTPIN, DHT22);
WiFiClient client;

void setup() { 
  pinMode(DHTPIN,INPUT);               
  Serial.begin(115200);
  delay(10);
  dht.begin();
  
  WiFi.begin(ssid, password);
 
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
   
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
