extern "C" 
{
  #include "user_interface.h"
}

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Time.h>
#include <TimeLib.h>
#include <Timer.h>
#include <Thingplus.h>
#include <WiFiClient.h>
#include <DHT.h>


//////////////////////////////////////////////////////////////////
const char *ssid =      "Up to the SKY";					                              	
const char *password =  "1402036768";			     
                       		
const char *apikey =    "x107do5ze2eLpg0oDw2trDwk2as=";  	
const char *humiId =    "percent-ecfabc0820b4-0";
const char *ledId =     "led-ecfabc0820b4-0";		                  	
const char *tempId =    "temperature-ecfabc0820b4-0";	      
//////////////////////////////////////////////////////////////////

int LED_GPIO = 7;            //LED 신호 입력 핀 설
int DHT_GPIO = 5;
int reportIntervalSec = 60;

Timer t;

int ledOffTimer = 0;
int ledBlinkTimer = 0;

static WiFiClient wifiClient;

DHT dht(DHT_GPIO,DHT22);

static void _serialInit(void)
{
	Serial.begin(115200);
	while (!Serial);// wait for serial port to connect.
	Serial.println();
}

static void _wifiInit(void)
{
#define WIFI_MAX_RETRY 150

  ESP.wdtDisable();
	WiFi.mode(WIFI_STA);

  if(!WiFi.getAutoConnect())
  {
  	WiFi.begin(ssid, password);
  
  	Serial.print("[INFO] WiFi connecting to ");
  	Serial.println(ssid);
  }

  else
  {
    Serial.print("AutoConnect");
  }

	int retry = 0;
	while (WiFi.status() != WL_CONNECTED) {
		Serial.print(retry++);
		Serial.print(".");
		if (!(retry % 50))
		  Serial.println();
		if (retry == WIFI_MAX_RETRY) {
			Serial.println();
			Serial.print("[ERR] WiFi connection failed. SSID:");
			Serial.print(ssid);
			Serial.print(" PASSWD:");
			Serial.println(password);
			while (1) {
				yield();
			}
		}
		delay(100);
	}

	Serial.println();
	Serial.print("[INFO] WiFi connected");
	Serial.print(" IP:");
	Serial.println(WiFi.localIP());
}

static void _gpioInit(void) 
{
	pinMode(LED_GPIO, OUTPUT);
}
static void _temphumiInit(void) 
{
  pinMode(DHT_GPIO, INPUT);
}


static void _ledOn(JsonObject& options) 
{
	int duration = options.containsKey("duration") ? options["duration"] : 0;

	digitalWrite(LED_GPIO, HIGH);

	if (duration)
		ledOffTimer = t.after(duration, _ledOff);
}

static void _ledOff(void) {
	t.stop(ledBlinkTimer);
	digitalWrite(LED_GPIO, LOW);
}

static bool _ledBlink(JsonObject& options) {
	if (!options.containsKey("interval")) {
		Serial.println(F("[ERR] No blink interval"));
		return false;
	}

	ledBlinkTimer = t.oscillate(LED_GPIO, options["interval"], HIGH);

	if (options.containsKey("duration"))
		ledOffTimer = t.after(options["duration"], _ledOff);

	return true;
}

char* actuatingCallback(const char *id, const char *cmd, JsonObject& options) {
	if (strcmp(id, ledId) == 0) { 
		if (strcmp(cmd, "on") == 0) {
			_ledOn(options);
			return "success";
		}
		else  if (strcmp(cmd, "off") == 0) {
			_ledOff();
			return "success";
		}
		else  if(strcmp(cmd, "blink") == 0) {
			if (_ledBlink(options) ) {
				return "success";
			}
			else {
				return NULL;
			}
		}
	}

	return NULL;
}

void setup() {
	_serialInit();

	uint8_t mac[6];
	WiFi.macAddress(mac);

	Serial.print("[INFO] Gateway Id:");
	Serial.println(WiFi.macAddress());

	_wifiInit();
	_gpioInit();
  _temphumiInit();

	Thingplus.begin(wifiClient, mac, apikey);
	Thingplus.actuatorCallbackSet(actuatingCallback);
	Thingplus.connect();
}


time_t current;
time_t nextReportInterval = now();

void updateTemp()
{
  float temp = dht.readTemperature();
  
  //Thingplus.sensorStatusPublish(tempId, true, reportIntervalSec * 2);
  Thingplus.valuePublish(tempId, temp);
}

void updateHum()
{
  float hum = dht.readHumidity();

  //Thingplus.sensorStatusPublish(humiId, true, reportIntervalSec * 2);
  Thingplus.valuePublish(humiId, hum);
}

void loop() 
{
	t.update();
	//system_soft_wdt_feed();

	current = now();
	if (current > nextReportInterval) 
	{
		Thingplus.gatewayStatusPublish(true, reportIntervalSec * 3);
		Thingplus.sensorStatusPublish(ledId, true, reportIntervalSec * 3);
		nextReportInterval = current + reportIntervalSec;

   updateTemp();
   updateHum();
	}
  Thingplus.loop();
}
