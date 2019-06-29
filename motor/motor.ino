#include<String.h>

int A_C = D3; 
int A_AC = D2; 
int B_C = D1;
int B_AC = D4;

String text;

void setup() 
{ 
  Serial.begin(9600);
  pinMode(A_C, OUTPUT); 
  pinMode(A_AC, OUTPUT);
  pinMode(B_C, OUTPUT);
  pinMode(B_AC, OUTPUT); 
  
} 
void loop() 
{ 
  if(Serial.available())
  {
    text = "null";
    text = Serial.readString();
    if(text != 0)
    {
      if(text == "f")
      {
        Serial.print(text);
        digitalWrite(A_C, HIGH); 
        digitalWrite(A_AC, LOW); 
        delay(1000); 
      }

      if(text == "b")f
      {
        Serial.print(text);
        digitalWrite(A_C, LOW); 
        digitalWrite(A_AC, HIGH); 
        delay(1000); 
      }

    } 
  }
  digitalWrite(A_C, LOW); 
  digitalWrite(A_AC, LOW);
}

