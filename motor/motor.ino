#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include<String.h>

char auth[] = "7bbaf5151f3e405ca95ab3e79d06f304";
char ssid[] = "Yubeen";
char pass[] = "01052366047";

int A_C = D3; 
int A_AC = D2; 
int B_C = D1;
int B_AC = D4;

String text;

void setup() 
{ 
  Serial.begin(9600);

  Blynk.begin(auth,ssid,pass);
  
  pinMode(A_C, OUTPUT); 
  pinMode(A_AC, OUTPUT);
  pinMode(B_C, OUTPUT);
  pinMode(B_AC, OUTPUT); 
  
} 
void loop() 
{ 
  //Blynk.run();
  motor();
}

void motor()
{
  text = "null";
  if(Serial.available())
  {
    text = Serial.readString();
      if(text == "f")
      {
        Serial.print(text);
        digitalWrite(A_C, HIGH); 
        digitalWrite(A_AC, LOW); 
        digitalWrite(B_C, LOW);
        digitalWrite(B_AC, HIGH);
        delay(100); 
      }

      if(text == "b")
      {
        Serial.print(text);
        digitalWrite(A_C, LOW); 
        digitalWrite(A_AC, HIGH); 
        digitalWrite(B_C, HIGH);
        digitalWrite(B_AC, LOW);
        delay(100); 
      } 
      if(text == "s")
      {
        Serial.print(text);
        digitalWrite(A_C, LOW); 
        digitalWrite(A_AC, LOW); 
        digitalWrite(B_C, LOW);
        digitalWrite(B_AC, LOW);
      }
  }
}

