#include "U8glib.h" // 라이브러리 선언
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE); // oled 모델명에 맞는 함수 선언
   
void setup() {
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


