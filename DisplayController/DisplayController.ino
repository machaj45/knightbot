
#include "Config.h"
#include "U8glib.h"


U8GLIB_ST7920_128X64_1X u8g(EN,RW,RS);  // SPI Com: SCK = en = 18, MOSI = rw = 16, CS = di = 17

void setup(void) {
  
  Serial.begin(SERIAL_BAUDRATE);
  // flip screen, if required
  // u8g.setRot180();
  
  // set SPI backup if required
  //u8g.setHardwareBackup(u8g_backup_avr_spi);

  // assign default color value
  if ( u8g.getMode() == U8G_MODE_R3G3B2 ) {
    u8g.setColorIndex(255);     // white
  }
  else if ( u8g.getMode() == U8G_MODE_GRAY2BIT ) {
    u8g.setColorIndex(3);         // max intensity
  }
  else if ( u8g.getMode() == U8G_MODE_BW ) {
    u8g.setColorIndex(1);         // pixel on
  }
  else if ( u8g.getMode() == U8G_MODE_HICOLOR ) {
    u8g.setHiColorByRGB(255,255,255);
  }
}

void loop(void) {
  // picture loop
  u8g.firstPage();  
  do {
    draw();
  } while( u8g.nextPage() );
  
  // rebuild the picture after some delay
  //delay(50);
}

void draw(void) {
  // graphic commands to redraw the complete screen should be placed here  
  //u8g.setFont(u8g_font_unifont);
  u8g.setFont(u8g_font_6x10);
  //u8g.setFont(u8g_font_osb21);
  u8g.drawStr( 10, 10, "(K)NightBot 0.1");
  u8g.drawStr( 0, 60, "waiting for ROSmaster");
}
