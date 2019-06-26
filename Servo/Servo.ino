#include <Servo.h> // 서보 모터 이용을 위한 라이브러

Servo servo_left;  // 왼쪽 서보 모터
Servo servo_right; // 오른쪽 서보 모터 

int angle = 0;  // 각도
int left_motor_pin = 8;  // 왼쪽 모터는 8번 핀
int right_motor_pin = 7; // 오른쪽 모터는 7번 핀

void setup() {
  Serial.begin(9600);

  //pinMode(left_motor_pin, OUTPUT);
  //pinMode(right_motor_pin, OUTPUT);

  servo_left.attach(left_motor_pin);   // 서보 모터 데이터 핀 연결해주기.
  servo_right.attach(right_motor_pin); // 서보 모터 데이터 핀 연겨해주기.

}

void loop() 
{
  char angle = Serial.read();
  servo_left.write(angle);
  servo_right.write(angle);
}
