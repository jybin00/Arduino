#include "U8glib.h"
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);
   
void setup() {
}
   
void loop() {
    u8g.firstPage();
    do{
        u8g.setFont(u8g_font_unifont);
        u8g.setPrintPos(8,15);
        u8g.print("JYB");
        u8g.drawStr(0,32, "X.position:");
        u8g.drawStr(0,43, "Y.position:");
        u8g.drawEllipse(20,10,20,10,U8G_DRAW_ALL);
    }while(u8g.nextPage());
}


