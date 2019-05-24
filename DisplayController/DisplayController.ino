
#include "Config.h"
#include "U8glib.h"


U8GLIB_ST7920_128X64_1X u8g(EN,RW,RS);  // SPI Com: SCK = en = 18, MOSI = rw = 16, CS = di = 17

boolean lastButtons[] = {false, false, false, false};
String lines[6];

boolean update = true;

void setup(void){
  
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
  pinMode(B0, INPUT_PULLUP);
  pinMode(B1, INPUT_PULLUP);
  pinMode(B2, INPUT_PULLUP);
  pinMode(B3, INPUT_PULLUP);

  pinMode(PWR_LED, OUTPUT);

  lines[0] = "  (K)NightBot 0.1";
  lines[3] = "waiting for ROSmaster";
  
  initCom();
}

void loop(void) {
  updateCom();

  if(update){
    u8g.firstPage();  
    do {
      draw();
    } while(u8g.nextPage());
  }
  
  updateButtons();
  
  delay(10);
}

void draw(void) {
  u8g.setFont(u8g_font_6x10);
  for(int i = 0; i < 5; i++){
    char line[21];
    lines[i].toCharArray(line, lines[i].length()+1);
    u8g.drawStr( 1, (i*10) + 9, line);
  }
  char line[21];
  lines[5].toCharArray(line, lines[5].length()+1);
  u8g.drawStr( 1, 62, line);
  
  u8g.drawLine(1, 51, 127, 51);
  u8g.drawLine(31, 51, 31, 63);
  u8g.drawLine(63, 51, 63, 63);
  u8g.drawLine(95, 51, 95, 63);
  update = false;
}

void updateButtons(){
  boolean buttons[4];
  buttons[0] = (digitalRead(B0) == LOW) ? true : false;
  buttons[1] = (digitalRead(B1) == LOW) ? true : false;
  buttons[2] = (digitalRead(B2) == LOW) ? true : false;
  buttons[3] = (digitalRead(B3) == LOW) ? true : false;
  
  for(int i = 0; i < 4; i++){
    if(lastButtons[i] && !buttons[i]){
      Serial.print("B");
      Serial.print(i);
    }
    lastButtons[i] = buttons[i];
  }
}
