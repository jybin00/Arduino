#include <Servo.h> // 서보 모터 이용을 위한 라이브러

Servo servo_left;  // 왼쪽 서보 모터
Servo servo_right; // 오른쪽 서보 모터 

int angle_left = 20;     // 값을 받아서 모터에 넣어주는 각도
int angle_right = 70;    // 20에 20이 기본 세팅
int angle_ini = 0;  // 초기 각도
int left_motor_pin = 8;  // 왼쪽 모터는 8번 핀
int right_motor_pin = 7; // 오른쪽 모터는 7번 핀

void setup() {
  Serial.begin(9600);

  //pinMode(left_motor_pin, OUTPUT);
  //pinMode(right_motor_pin, OUTPUT);

  servo_left.attach(left_motor_pin);   // 서보 모터 데이터 핀 연결해주기.
  servo_right.attach(right_motor_pin); // 서보 모터 데이터 핀 연겨해주기.

  servo_left.write(angle_left);   // 서보모터 초기화
  servo_right.write(angle_right);  // 서보모터 초기화
}

void Servo_on()
{
  servo_left.attach(left_motor_pin);   // 서보 모터 데이터 핀 연결해주기.
  servo_right.attach(right_motor_pin); // 서보 모터 데이터 핀 연겨해주기.
}

void Servo_off()
{
  servo_left.detach();   // 서보 모터 데이터 핀 연결해주기.
  servo_right.detach(); // 서보 모터 데이터 핀 연겨해주기.
}

void loop() 
{
  if(Serial.available())
  {
    int angle_left = Serial.parseInt();
    int angle_right = Serial.parseInt();
    Serial.print("angle_left: ");
    Serial.println(angle_left);
    Serial.print("angle_right: ");
    Serial.println(angle_right);

    Servo_on();
    
    servo_left.write(angle_left);
    delay(500);
    servo_right.write(angle_right);
    delay(420);
    
    Servo_off();
    delay(1000);
  }
 
}
