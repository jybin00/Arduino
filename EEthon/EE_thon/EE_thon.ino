///최종 모음///

#include <SoftwareSerial.h> //시리얼 통신 라이브러리 호출
#include <Servo.h>
///블루투스 PIN 설정////////////
#define BLUE_TX 2 //블루투스 TX
#define BLUE_RX 3 //블루투스 RX
//////////////////////////////
/// 모터 드라이버 ////////////////////////////////
#define IN1 A3//L298모듈의 제어 신호 입력 핀 번호 지정
#define IN2 A2
#define IN3 8
#define IN4 9
#define EN_A 5//PIN 번호 설정
#define EN_B 10//PIN 번호 설정
#define Speed 100
////////////////////////////////////////////////
/// 초음파 센서 ////////////////////
#define trigPin_left 11
#define echoPin_left 12
#define trigPin_middle 7
#define echoPin_middle 6
#define trigPin_right A5
#define echoPin_right A4
/////////////////////////////////
float duration_left,duration_middle,duration_right; //// 초음파 센서 시간
float distance_left,distance_middle,distance_right; //// 초음파 센서 거리

#define servoPin 4 //서보모터 연결 포트
Servo servo;
 
SoftwareSerial mySerial(BLUE_TX, BLUE_RX);  //시리얼 통신을 위한 객체선언
char value; //블루투스에서 받는 값
///////동작들//////////////////////////////////
void forward() { /// 전진
  digitalWrite(IN1,HIGH);
  digitalWrite(IN2,LOW);
  digitalWrite(IN3,HIGH);
  digitalWrite(IN4,LOW);
  analogWrite(EN_A, Speed); //A모터 조절
  analogWrite(EN_B, Speed); //B모터 조절
}
void back() {  //후진
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,HIGH);
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,HIGH);
  analogWrite(EN_A, Speed); //A모터 조절
  analogWrite(EN_B, Speed); //B모터 조절
}
void left() {  //좌회전(왼쪽 멈춤, 오른쪽 전진)
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,LOW);
  digitalWrite(IN3,HIGH);
  digitalWrite(IN4,LOW);
  analogWrite(EN_A, Speed); //A모터 조절
  analogWrite(EN_B, Speed); //B모터 조절
}
void right() {  //우회전(왼쪽 전진, 오른쪽 멈춤)
  digitalWrite(IN1,HIGH);
  digitalWrite(IN2,LOW);
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,LOW);
  analogWrite(EN_A, Speed); //A모터 조절
  analogWrite(EN_B, Speed); //B모터 조절
}
void Stop() {   //정지
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,LOW);
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,LOW);
  analogWrite(EN_A, Speed); //A모터 조절
  analogWrite(EN_B, Speed); //B모터 조절
}
void left_sensor() // 왼쪽 센서
{
  digitalWrite(trigPin_left,LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin_left,HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin_left,LOW);
   
  duration_left = pulseIn(echoPin_left,HIGH);
  distance_left = (duration_left/2)/29.1;
}
void middle_sensor()  // 가운데 코드
{
  digitalWrite(trigPin_middle,LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin_middle,HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin_middle,LOW);
  
  duration_middle = pulseIn(echoPin_middle,HIGH);
  distance_middle = (duration_middle/2)/29.1;
}
void right_sensor() // 오른쪽 센서
{
  digitalWrite(trigPin_right,LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin_right,HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin_right,LOW);
  
  duration_right = pulseIn(echoPin_right,HIGH);
  distance_right = (duration_right/2)/29.1;
}
///////////////////////////////////////////////////
 
void setup() {
  Serial.begin(9600);   //시리얼모니터
  mySerial.begin(9600); //블루투스 시리얼 개방
 
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  //초음파
 
  pinMode(trigPin_left,OUTPUT);
  pinMode(echoPin_left,INPUT);
  pinMode(trigPin_middle,OUTPUT);
  pinMode(echoPin_middle,INPUT);
  pinMode(trigPin_right,OUTPUT);
  pinMode(echoPin_right,INPUT);
  servo.attach(servoPin);
  servo.write(90);
  delay(1000);
 
  Serial.println("Arduino Bluetooth Car is ready to start");
 
}

void loop() {
  while(mySerial.available())  //mySerial에 전송된 값이 있으면
  {
  value = mySerial.read(); //전송값 읽음
  Serial.print(value);
  while(value == '0')  //자율주행모드
  {   
    if( value == '\0') // 0이후 아무 값도 입력되지 않는 경우
    {
      left_sensor();
      middle_sensor();
      right_sensor();
      delay(100);
        if( distance_left < 13 && distance_middle < 13 && distance_right < 13 ) // 기준 거리 설정 필요
          {
                Stop();
          }
        if( distance_left < 13 && distance_middle < 13 ) // 기준 거리 설정 필요
          { Stop();
            delay(300);
            back();
            delay(300);
            servo.write(120); //방향
            delay(300);
          }
        if( distance_middle < 13 && distance_right < 13 ) // 기준 거리 설정 필요
          { Stop();
          delay(300);
          back();
          delay(300);
          servo.write(60); //방향
          delay(300);
        }
        if( distance_middle < 13 ) // 기준 거리 설정 필요
        { 
          Stop();
          delay(300);
          back();
          delay(300);
          int rnd = random(0,2);
            if (rnd == 0)
              {
                right(); 
              }
          } 
          else
              {
              forward();
              }
    }
    else if(value == '5')
    {
      break;
      }
    }
  }
    
  if(value == '1'){    //좌회전
    Serial.println("left");
    left();
  }else if(value == '2'){   //우회전
    Serial.println("right");
    right();
  }else if(value == '3'){   //전진
    Serial.println("forward");
    forward();
  }else if(value == '4'){   //후진
    Serial.println("backward");
    back();
  }
  }
  
 


