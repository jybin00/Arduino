// 핸즈 프로젝트 "집에 가고 싶어" RC카 code
//
// 작성자 : 승민,영준,하영,유빈
//

#include <SSD1306Wire.h>

SSD1306Wire display(0x3c, 21,22);


// 스피드 설정 0.15초 마다 1스텝씩 이동
#define DEFAULT_SPEED 60

// 모터 방향 설정
#define FORWARD 1
#define BACKWARD 2
#define LEFT 3
#define RIGHT 4

// 모터 제어를 위한 타이머 설정
unsigned long LeftMotorTimer, RightMotorTimer, systemTimer;

// 스텝 모터 작동을 위한 함수 설정
#define RightStep(a,b,c,d) digitalWrite(26,a); digitalWrite(27,b); digitalWrite(14,c); digitalWrite(12,d); 
#define LeftStep(a,b,c,d) digitalWrite(19,a); digitalWrite(18,b); digitalWrite(4,c); digitalWrite(0,d);

int LeftMotorDir, RightMotorDir;  // 왼쪽 모터와 오른쪽 모터 방향 변수

int LeftMotorStepIndex = 0;  // 왼쪽 모터 제어용 인덱스
int RightMotorStepIndex = 0; // 오른쪽 모터 제어용 인덱스

int LeftStepIndex = 0;  // 왼쪽 모터 스텝 저장용 변수
int RightStepIndex = 0; // 오른쪽 모터 스텝 저장용 변수

int Distance[3];  // 거리 저장을 위한 배열

void setup() {
  Serial.begin(115200);

  display.init();
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);

///// 스텝모터 초기 설정
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

  LeftMotorDir = FORWARD;
  RightMotorDir = FORWARD;

}

void LeftMotorStep() // 왼쪽 모터 동작을 위한 함수 
{
  if(LeftMotorDir == FORWARD)
  {
    if(LeftMotorStepIndex == 3) 
    {
      LeftMotorStepIndex = 0; // 인덱스가 3이면 0으로 초기화
      LeftStepIndex++;
    }
    else 
    {
      LeftMotorStepIndex++;
      LeftStepIndex++;
    }
  }
  else if(LeftMotorDir == BACKWARD)
  {
    if(LeftMotorStepIndex == 0)
    {
      LeftMotorStepIndex = 3;
    }
    else
    {
      LeftMotorStepIndex--;
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
    if(RightMotorStepIndex == 3)
    {
      RightMotorStepIndex = 0;
    }
    else
    {
      RightMotorStepIndex++;
    }
  }
  else if(RightMotorDir == BACKWARD)
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

void drawFontFaceDemo() {
  // clear the display
  display.clear();
    // Font Demo1
    // create more fonts at http://oleddisplay.squix.ch/
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.setFont(ArialMT_Plain_10);
    display.drawString(0, 0, "Hello world");
    display.setFont(ArialMT_Plain_16);
    display.drawString(0, 10, "Hello world");
    display.setFont(ArialMT_Plain_24);
    display.drawString(0, 26, "Hello world");
  // write the buffer to the display
  display.display();
}


void loop() 
{ // 스피드로 설정한 시간이 지날 때마다 실행
  if(millis() >= LeftMotorTimer + DEFAULT_SPEED)  
  {
    LeftMotorTimer = millis();
    LeftMotorStep();
  }

  if(millis() >= RightMotorTimer + DEFAULT_SPEED)
  {
    RightMotorTimer = millis();
    RightMotorStep();
  }

  if(millis() >= systemTimer + 1000)
  {
    systemTimer = millis();
    Serial.println(LeftStepIndex);
  }

  drawFontFaceDemo();
  delay(1000);

}
