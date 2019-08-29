// 서브 모터 //

#include <Servo.h>
Servo servo;
int angle = 90;
int i = 0;
void setup()
{
  Serial.begin(9600);
  servo.attach(2);
}

void loop() {
  if(Serial.available())
  {
  servo.attach(2);
  angle = Serial.parseInt();
  if( angle <= 170 && angle >= 20)
  {
    Serial.println(angle);
    servo.write(angle);
    delay(100);
    servo.detach();
    delay(10);
  }
  }
}


