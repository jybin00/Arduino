// 핸즈 프로젝트 "집에 가고 싶어" RC카 code
//
// 작성자 : 승민,영준,하영,유빈
//

#include <SSD1306Wire.h>  // 0.96 inch OLED 사용을 위한 라이브러리
#include <math.h>         // 거리 및 각도 계산 함수


SSD1306Wire display(0x3c, 21,22);  // 0x3c는 메모리 주소 // 21 == SDA 22 == SCL


// 스피드 설정 0.15초 마다 1스텝씩 이동
#define DEFAULT_SPEED 15

// 모터 방향 설정
#define FORWARD 1
#define BACKWARD 2
#define LEFT 3
#define RIGHT 4

// 모터 제어를 위한 타이머 설정
unsigned long LeftMotorTimer, RightMotorTimer, systemTimer;  // 시스템 타이머는 OLED 업데이트 용도

// 스텝 모터 작동을 위한 함수 설정
#define RightStep(a,b,c,d) digitalWrite(26,a); digitalWrite(27,b); digitalWrite(14,c); digitalWrite(12,d); // blue,green, red, black
#define LeftStep(a,b,c,d) digitalWrite(19,a); digitalWrite(18,b); digitalWrite(4,c); digitalWrite(0,d); // 위와 동일

int LeftMotorDir, RightMotorDir;  // 왼쪽 모터와 오른쪽 모터 방향 변수

int LeftMotorStepIndex = 0;  // 왼쪽 모터 제어용 인덱스
int RightMotorStepIndex = 0; // 오른쪽 모터 제어용 인덱스

int LeftStepIndex = 0;  // 왼쪽 모터 스텝 저장용 변수
int RightStepIndex = 0; // 오른쪽 모터 스텝 저장용 변수


int RC_Move_Order = 0;  //차량 제어 명령
int RC_Car_Dir = 1;     //차체의 방향 ( 앞 부분이 어디를 향하고 있나를 나타네는 변수 ) 

int X_Pos = 0;    // x위치 저장용 변수
int Y_Pos = 0;    // y위치 저장용 변수
int Distance = 0; // 거리 저장용 변수
float radian = 0; // 각 계산을 위한 변수
float degree = 0;

void setup() 
{
  // 시리얼 통신 시작 
  Serial.begin(115200);

  // OLED 함수
  display.init(); // OLED 초기화
  display.flipScreenVertically(); // OLED 수직으로 뒤집기
  display.setFont(Open_Sans_SemiBold_13); // OLED 폰트 설정

  // 스텝모터 초기 설정
  pinMode(26, OUTPUT);
  pinMode(27, OUTPUT);
  pinMode(14, OUTPUT);
  pinMode(12, OUTPUT);

  pinMode(19, OUTPUT);
  pinMode(18, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(0, OUTPUT);

  LeftMotorTimer = millis();
  RightMotorTimer = millis() + 10; // 제어용 타이머 충돌 방지 
  systemTimer = millis();

  // 초기 모터 방향은 forward
  LeftMotorDir = FORWARD;
  RightMotorDir = FORWARD;

}

void LeftMotorStep() // 왼쪽 모터 동작을 위한 함수
{        
  if(LeftMotorDir == FORWARD) 
  {
    if(LeftMotorStepIndex == 0) 
    {
      LeftMotorStepIndex = 3; // 인덱스가 3이면 0으로 초기화
    }
    else 
    {
      LeftMotorStepIndex--;
    }
  }
  else if(LeftMotorDir == BACKWARD)
  {
    if(LeftMotorStepIndex == 3)
    {
      LeftMotorStepIndex = 0;
    }
    else
    {
      LeftMotorStepIndex++;
    }
  }
  else
  {
    LeftStep(0,0,0,0);
    return;
  }
  switch(LeftMotorStepIndex)
  {
    case 0:
      LeftStep(1,0,0,0);
      break;
    case 1:
      LeftStep(0,1,0,0);
      break;
    case 2:
      LeftStep(0,0,1,0);
      break;
    case 3:
      LeftStep(0,0,0,1);
      break;
  }
}

void RightMotorStep() // 오른쪽 모터 제어를 위한 함수
{
  if(RightMotorDir == FORWARD) 
  {
    if(RightMotorStepIndex == 0)
    {
      RightMotorStepIndex = 3;
    }
    else
    {
      RightMotorStepIndex--;
    }
  }
  else if(RightMotorDir == BACKWARD) 
  {
    if(RightMotorStepIndex == 3)
    {
      RightMotorStepIndex = 0;
    }
    else
    {
      RightMotorStepIndex++;
    }
  }
  else
  {
    RightStep(0,0,0,0);
    return;
  }
    
  switch(RightMotorStepIndex) 
  {
    case 3:
      RightStep(1,0,0,0);
      break;
    case 2:
      RightStep(0,1,0,0);
      break;
    case 1:
      RightStep(0,0,1,0);
      break;
    case 0:
      RightStep(0,0,0,1);
      break;
  }
}

void RC_Car_Info() { // OLED 함수
  display.clear();
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.drawString(0, 0, "x : ");
    display.drawString(0, 13, "y : ");
    display.drawString(0, 26, "distance : ");
    display.drawString(30, 0, String(X_Pos));
    display.drawString(30, 13, String(Y_Pos));
    display.drawString(70, 26, String(Distance));
  display.display();
}

void Forward() 
{
  RightMotorDir = FORWARD;
  LeftMotorDir = FORWARD;

  for(int i=0; i<=200; i++) 
  {
    LeftMotorStep();
    RightMotorStep();
    delay(DEFAULT_SPEED);
  }
}

void Backward() 
{
  RightMotorDir = BACKWARD;
  LeftMotorDir = BACKWARD;

  for(int i=0; i<=200; i++) 
  {
    LeftMotorStep();
    RightMotorStep();
    delay(DEFAULT_SPEED);
  }
}

void Left() 
{
  RightMotorDir = FORWARD;
  LeftMotorDir = BACKWARD;

  for(int i=0; i<=132; i++) 
  {
    LeftMotorStep();
    RightMotorStep();
    delay(DEFAULT_SPEED);
  }
}

void Right() 
{
  RightMotorDir = BACKWARD;
  LeftMotorDir = FORWARD;

  for(int i=0; i<=132; i++) 
  {
    LeftMotorStep();
    RightMotorStep();
    delay(DEFAULT_SPEED);
  }
}

void RC_Car_Pos() // RC car가 움직일 때 위치 저장을 위한 함수 ( 이동 함수 + 위치 저장 함수 )
{           
  switch(RC_Car_Dir) 
  {
    case 1:  // Foward 일 때
      switch(RC_Move_Order)
      {
        case 1: //Forward
          Y_Pos += 200;
          Forward();
          break;
        case 2: //Backward
          Y_Pos -= 200;
          Backward();
          break;
        case 3: //Left
          RC_Car_Dir = 3;
          Left();
          break;
        case 4: //Right
          RC_Car_Dir = 4;
          Right();
          break; 
      }
    break;
          
    case 2: // Backward
    
      switch(RC_Move_Order) 
      {
        case 1: //Forward
          Y_Pos -= 200;
          Forward();
          break;
        case 2: //Backward
          Y_Pos += 200;
          Backward();
          break;
        case 3: //Left
          RC_Car_Dir = 4;
          Left();
          break;
        case 4: //Right
          RC_Car_Dir = 3;
          Right();
          break; 
      }
      break;

    case 3: // Left
      switch(RC_Move_Order)
      {
        case 1: //Forward
          X_Pos -= 200;
          Forward();
          break;
        case 2: //Backward
          X_Pos += 200;
          Backward();
          break;
        case 3: //Left
          RC_Car_Dir = 2;
          Left();
          break;
        case 4: //Right
          RC_Car_Dir = 1;
          Right();
          break; 
      }
      break;

    case 4: // Right
      switch(RC_Move_Order)
      {
        case 1: //Forward
          X_Pos += 200;
          Forward();
          break;
        case 2: //Backward
          X_Pos -= 200;
          Backward();
          break;
        case 3: //Left
          RC_Car_Dir = 1;
          Left();
          break;
        case 4: //Right
          RC_Car_Dir = 2;
          Right();  
          break; 
      }
      break; 
   } 
}

void distance() 
{
  Distance = sqrt(pow(X_Pos,2)+pow(Y_Pos,2));
}

void Return_Angle() 
{
  radian = atan2(Y_Pos,X_Pos);
  degree = (57.29578*radian); 
}


void loop() 
{ 
  if(Serial.available()) 
  {
    RC_Move_Order = Serial.parseInt();
    RC_Car_Pos();
    distance();
    Return_Angle();
    Serial.print("X_Pos : ");
    Serial.println(X_Pos);
    Serial.print("Y_Pos : ");
    Serial.println(Y_Pos);
    Serial.print("Distance : ");
    Serial.println(Distance);
    Serial.print("Angle : ");
    Serial.println(degree);
  }
  
  if(millis() >= systemTimer + 1000)
  {
    systemTimer = millis();
    //Serial.println(LeftStepIndex);
    RC_Car_Info();
  }

}
