#include "DHT.h" // DHT-22 센서를 사용하기 위한 라이브러리를 불러온다
#include "U8glib.h" // 라이브러리 선언
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE); // oled 모델명에 맞는 함수 선언
   
void setup() {        // 처음 한 번만 실행됨.
  Serial.begin(9600); // 시리얼 통신 시작. 통신 속도는 9600!
}
   
void loop() {
    u8g.firstPage(); // 첫번째 페이지
    do{
        u8g.setFont(u8g_font_unifont); // OLED 폰트 설정
        u8g.setPrintPos(8,15); // 프린트할 글자 위치 설정
        u8g.print("JYB"); // 글자 프린트
        u8g.drawStr(0,32, "X.position:"); // x위치 프린트
        u8g.drawStr(0,43, "Y.position:"); // y위치 프린트 
        u8g.drawEllipse(20,10,20,10,U8G_DRAW_ALL); // 타원 그리기 , x,y,rx,ry,옵션
    }while(u8g.nextPage()); // 다음 페이지가 계속 나오면 (==1 이면)
}



/*습도를 측정할 때, 상부 전극과 하부 전극 사이의 저항 변화를 측정함으로써 공기중의 습도 변화를 
       알아낼 수 있습니다. 박막 폴리머(습도 흡착판)의 양쪽 표면에는 전극이 부착된 얇은 판이 있는데, 
       이 판이 공기중의 수분을 흡수합니다. 박막 폴리머(습도 흡착판)의 표면에 부착된 수분 양에 의해 
       두 전극의 전도도에 변화가 일어나게 되면 이를 감지하는 방식으로 습도를 출력합니다.
[출처] [아두이노 강좌] DHT-22를 이용한 온도 및 습도 측정하기|작성자 에듀이노 오픈랩 */

/* 온도 측정 원리
        온도를 감지하는 부분은 반도체 세라믹으로 이루어져 온도에 따라 물질의 저항 값이 변하는 소재의         특성을 이용, 값의 변화를 감지해 온도를 출력하고 있습니다.
[출처] [아두이노 강좌] DHT-22를 이용한 온도 및 습도 측정하기|작성자 에듀이노 오픈랩 */


