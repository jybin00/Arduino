// 핸즈 프로젝트 "집에 가고 싶어" RC카 code
//
// 작성자 : 승민,영준,하영,유빈
//

#include <SSD1306Wire.h>  // 0.96 inch OLED 사용을 위한 라이브러리
#include <math.h>         // 거리 및 각도 계산 함수

//Bluetooth//////////////////////////////////////////////////
#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT; 
///////////


SSD1306Wire display(0x3c, 21,22);  // 0x3c는 메모리 주소 // 21 == SDA 22 == SCL


// 스피드 설정 0.0037초 마다 1스텝씩 이동
#define DEFAULT_SPEED 3700

// 모터 방향 설정
#define FORWARD 1
#define BACKWARD 4
#define LEFT 2
#define RIGHT 3

// 모터 제어를 위한 타이머 설정
unsigned long LeftMotorTimer, RightMotorTimer, systemTimer, systemTimer2;  // 시스템 타이머는 OLED 업데이트 용도

// 스텝 모터 작동을 위한 함수 설정
#define RightStep(a,b,c,d) digitalWrite(26,a); digitalWrite(27,b); digitalWrite(14,c); digitalWrite(12,d); // blue,green, red, black
#define LeftStep(a,b,c,d) digitalWrite(19,a); digitalWrite(18,b); digitalWrite(4,c); digitalWrite(0,d); // 위와 동일

int LeftMotorDir, RightMotorDir;  // 왼쪽 모터와 오른쪽 모터 방향 변수

int LeftMotorStepIndex = 0;  // 왼쪽 모터 제어용 인덱스
int RightMotorStepIndex = 0; // 오른쪽 모터 제어용 인덱스

int RC_Move_Order = 0;  //차량 제어 명령
int RC_Car_Dir = 1;     //차체의 방향 ( 앞 부분이 어디를 향하고 있나를 나타네는 변수 ) 

int X_Pos = 0;    // x위치 저장용 변수
int Y_Pos = 0;    // y위치 저장용 변수
int Distance = 0; // 거리 저장용 변수
int i,j = 0; // 돌아올 때 쓰이는 각도 인덱스
float radian = 0; // 각 계산을 위한 변수
float degree = 0;


void Return_Home();

void setup() 
{
  // 시리얼 통신 시작 
  Serial.begin(115200);
  
  //bluetooth
  SerialBT.begin("ESP32test"); //Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");

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
      LeftMotorStepIndex = 3; // 인덱스가 3이면 0으로 초기화
    else 
      LeftMotorStepIndex--;
  }
  else if(LeftMotorDir == BACKWARD)
  {
    if(LeftMotorStepIndex == 3)
      LeftMotorStepIndex = 0;
    else
      LeftMotorStepIndex++;
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
    display.drawString(0, 39, "angle : ");
    display.drawString(30, 0, String(X_Pos));
    display.drawString(30, 13, String(Y_Pos));
    display.drawString(70, 26, String(Distance));
    display.drawString(70, 39, String(degree));
    
  display.display();
}

void Forward() 
{
  RightMotorDir = FORWARD;
  LeftMotorDir = FORWARD;

  for(int i=0; i<=300; i++) 
  {
    LeftMotorStep(),RightMotorStep();
    delayMicroseconds(DEFAULT_SPEED);
  }
}

void Backward() 
{
  RightMotorDir = BACKWARD;
  LeftMotorDir = BACKWARD;

  for(int i=0; i<=300; i++) 
  {
    LeftMotorStep(),RightMotorStep();
    delayMicroseconds(DEFAULT_SPEED);
  }
}

void Left() 
{
  RightMotorDir = FORWARD;
  LeftMotorDir = BACKWARD;

  for(int i=0; i<=110; i++) 
  {
    LeftMotorStep(),RightMotorStep();
    delayMicroseconds(DEFAULT_SPEED);
  }
}

void Right() 
{
  RightMotorDir = BACKWARD;
  LeftMotorDir = FORWARD;

  for(int i=0; i<=110; i++) 
  {
    LeftMotorStep(),RightMotorStep();
    delayMicroseconds(DEFAULT_SPEED);
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
          Y_Pos += 300;
          Forward();
          break;
        case 4: //Backward
          Y_Pos -= 300;
          Backward();
          break;
        case 2: //Left
          RC_Car_Dir = 2;
          Left();
          break;
        case 3: //Right
          RC_Car_Dir = 3;
          Right();
          break; 
        case 5: // return to the home
          Return_Home();
          break;
      }
    break;
          
    case 4: // Backward
    
      switch(RC_Move_Order) 
      {
        case 1: //Forward
          Y_Pos -= 300;
          Forward();
          break;
        case 4: //Backward
          Y_Pos += 300;
          Backward();
          break;
        case 2: //Left
          RC_Car_Dir = 3;
          Left();
          break;
        case 3: //Right
          RC_Car_Dir = 2;
          Right();
          break;
        case 5: // return to the home
          Return_Home();
          break; 
      }
    break;

    case 2: // Left
      switch(RC_Move_Order)
      {
        case 1: //Forward
          X_Pos -= 300;
          Forward();
          break;
        case 4: //Backward
          X_Pos += 300;
          Backward();
          break;
        case 2: //Left
          RC_Car_Dir = 4;
          Left();
          break;
        case 3: //Right
          RC_Car_Dir = 1;
          Right();
          break; 
        case 5: // return to the home
          Return_Home();
          break;
      }
    break;

    case 3: // Right
      switch(RC_Move_Order)
      {
        case 1: //Forward
          X_Pos += 300;
          Forward();
          break;
        case 4: //Backward
          X_Pos -= 300;
          Backward();
          break;
        case 2: //Left
          RC_Car_Dir = 1;
          Left();
          break;
        case 3: //Right
          RC_Car_Dir = 4;
          Right();  
          break; 
        case 5: // return to the home
          Return_Home();
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

void Return_Home()
{
  //각도 계산
  float numAngle[4][4] = {
    {270-degree, 270-degree,  90-degree, 270-degree},
    {360-degree, 360-degree, 180-degree,    -degree},
    {180-degree, 180-degree, 360-degree, 180-degree}, 
    { 90-degree, 450-degree, 270-degree,  90-degree}
  };
  
  //각도 바꾸는 스텝 수
  i = 0; 
  j = RC_Car_Dir - 1;
  if(X_Pos>=0) 
  {
    if(Y_Pos>=0) i = 0;
    else i = 3;
  } 
  else 
  {
    if(Y_Pos>=0) i= 1; 
    else i = 2; 
  }

  int acs = (int) (numAngle[j][i]*1.19);
  
  //각도 바꾸기
  RightMotorDir = BACKWARD;
  LeftMotorDir = FORWARD;

  for(int i=0; i<=acs; i++) 
  {
    LeftMotorStep();
    RightMotorStep();
    delayMicroseconds(DEFAULT_SPEED);
  }

  delay(20);
  
  //거리 계산 및 거리 스텝 수
  RightMotorDir = FORWARD;
  LeftMotorDir = FORWARD;

  for(int j=0; j<=(int) Distance; j++) 
  {
    LeftMotorStep();
    RightMotorStep();
    delayMicroseconds(DEFAULT_SPEED);
  }

  switch(i)
    {
    case 1 :
      RightMotorDir = BACKWARD;
      LeftMotorDir = FORWARD;
    Serial.println(i);
      for(int i=0; i<=(int) (degree-90)*1.19; i++) 
      {
        LeftMotorStep();
        RightMotorStep();
        delayMicroseconds(DEFAULT_SPEED);
      } 
    break;

    case 2 :
      RightMotorDir = FORWARD;
      LeftMotorDir = BACKWARD;
    Serial.println(i);
      for(int i=0; i<=(int) (270-degree)*1.19; i++) 
      {
        LeftMotorStep();
        RightMotorStep();
        delayMicroseconds(DEFAULT_SPEED);
      } 
    break;
    
    case 3 :
      RightMotorDir = BACKWARD;
      LeftMotorDir = FORWARD;
    Serial.println(i);
      for(int i=0; i<=(int) (degree-270)*1.19; i++) 
      {
        LeftMotorStep();
        RightMotorStep();
        delayMicroseconds(DEFAULT_SPEED);
      } 
    break;

    case 4 :
      RightMotorDir = FORWARD;
      LeftMotorDir = BACKWARD;
    Serial.println(i);
      for(int i=0; i<=(int) (270-degree)*1.19; i++) 
      {
        LeftMotorStep();
        RightMotorStep();
        delayMicroseconds(DEFAULT_SPEED);
      } 
    break;
    }
  delay(1);
  
  X_Pos = 0;
  Y_Pos = 0; 
  Distance = 0;
  degree = 0;
  RC_Car_Dir = 1;
}


void loop() 
{ 
  if(SerialBT.available()) 
  {
    delay(1);
    RC_Move_Order = SerialBT.parseInt();
    RC_Car_Pos();
    distance();
    Return_Angle();
    
    /*
    Serial.print("X_Pos : ");
    Serial.println(X_Pos);
    Serial.print("Y_Pos : ");
    Serial.println(Y_Pos);
    Serial.print("Distance : ");
    Serial.println(Distance);
    Serial.print("Angle : ");
    Serial.println(degree);*/
  }
  
  if(millis() >= systemTimer + 1000)
  {
    systemTimer = millis();
    RC_Car_Info();
  } 

}
