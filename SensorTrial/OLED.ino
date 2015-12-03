//#include <Adafruit_GFX.h>
//#include <Adafruit_SSD1306.h>

#include "U8glib.h"

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_DEV_0|U8G_I2C_OPT_NO_ACK|U8G_I2C_OPT_FAST);  // Fast I2C / TWI 

void SetupOLED()
{
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

void setOrientation(float roll)
{
  if (roll < 0)  { u8g.setRot180(); }
  else           { u8g.undoRotation(); }
}

void draw(void) {
  String Str;
  const char * str;
  
  // graphic commands to redraw the complete screen should be placed here  
  u8g.setFont(u8g_font_04b_03r);
  //u8g.setFont(u8g_font_osb21);
  u8g.drawStr( 0, 5, "**SENSOR TRIAL**");

    u8g.drawStr( 0, 20, "Accel:");
    Str = getStrAcceleration();
    str = Str.c_str();
    u8g.drawStr( 0, 27, str);

    u8g.drawStr( 0, 40, "Orient:");
    Str = getStrOrientation();
    str = Str.c_str();
    u8g.drawStr( 0, 47, str);

    Str = getStrBMP();
    str = Str.c_str();
    u8g.drawStr( 0, 61, str);
}

void taskOLED(void) {
  // picture loop
  u8g.firstPage();  
  do {
    draw();
  } while( u8g.nextPage() );
  
  // rebuild the picture after some delay
  delay(50);
}

