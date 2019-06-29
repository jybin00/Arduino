import processing.serial.*;  // 시리얼 통신을 위한 라이브러리

Serial myPort;  // 시리얼 객체 선언
String myString = null; // 문자열 변수 선언과 초기화

int humi = 0;  // 습도 변수 선언 및 초기화
int temp = 0;  // 온도 변수 선언 및 초기화

void setup()
{
  size(1200,1000);
  background(0);
  printArray(Serial.list());   // 이 함수를 입력하면 포트 값을 알아낼 수 있음.
  myPort = new Serial(this,Serial.list()[1],9600); // 시리얼 통신 시작 함수같은 거 (잘 모르겠음.)
}

void draw()  // 무언가 그리는 함수
{
  drawLine();
}


void serialEvent(Serial myPort)   // 시리얼을 통해서 데이터가 들어올 때만 실행됨.
{
   //if(myPort.available() > 0)  // 따라서 이 구문이 필요가 없음.
    try
    {
      myString = myPort.readStringUntil('\n');  // 개행 문자 전까지 읽기! 중요!
      String[] list = split(myString,' '); // 받아온 문자열을 띄어쓰기로 나누기
      temp = int(list[0]);                 // 온도 값을 정수형으로 변환
      humi = int(list[1]);                 // 습도 값을 정수형으로 변환
      
      print("Temp : " +temp + "도");        // 온도 값을 출력
      println(" humi : "+humi + "%");      // 습도 값을 출력
    }catch (Exception e)  // exception 에러가 뜨면 그냥 지나가라
    {
      
    }
}

void drawLine()
{
 pushMatrix();
 
 translate(width/2, height);
 strokeWeight(4);
 stroke(98, 245, 31);
 

 
 popMatrix();
}
