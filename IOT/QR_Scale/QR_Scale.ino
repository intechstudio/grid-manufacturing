/**
 *  QRCode
 *
 *  A quick example of generating a QR code.
 *
 *  This prints the QR code to the serial monitor as solid blocks. Each module
 *  is two characters wide, since the monospace font used in the serial monitor
 *  is approximately twice as tall as wide.
 *
 */

#include "qrcode.h"
#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE, /* clock=*/ 16, /* data=*/ 17);   // ESP32 Thing, HW I2C with pin remapping


// End of constructor list



uint32_t i = 0;

void u8g2_prepare(void) {
  u8g2.setFont(u8g2_font_6x10_tf);
  u8g2.setFontRefHeightExtendedText();
  u8g2.setDrawColor(1);
  u8g2.setFontPosTop();
  u8g2.setFontDirection(0);
}

void u8g2_box_frame(uint8_t a) {
  u8g2.drawStr( 0, 0, "drawBox");
  u8g2.drawBox(5,10,20,10);
  u8g2.drawBox(10+a,15,30,7);
  u8g2.drawStr( 0, 30, "drawFrame");
  u8g2.drawFrame(5,10+30,20,10);
  u8g2.drawFrame(10+a,15+30,30,7);

  uint8_t str[30];

  sprintf(str, "W: %d", i);
  
  u8g2.drawStr(0, 22, str);
      
}




QRCode qrcode;
uint8_t qrcodeData[56];


void qrprint(void){

    uint8_t str[30];

    sprintf(str, "W: %d", i);
    qrcode_initText(&qrcode, qrcodeData, 1, ECC_HIGH, str);
    
    u8g2.drawStr(70, 22, str);

    //Serial.print("\n\n");

    for (uint8_t y = 0; y < qrcode.size; y++) {

        // Each horizontal module
        for (uint8_t x = 0; x < qrcode.size; x++) {
          if (qrcode_getModule(&qrcode, x, y) ){
            
//            u8g2.drawPixel(x*2,y*2);
//            u8g2.drawPixel(x*2+1,y*2);
//            u8g2.drawPixel(x*2,y*2+1);
//            u8g2.drawPixel(x*2+1,y*2+1);

            u8g2.drawPixel(x*3,y*3);
            u8g2.drawPixel(x*3+1,y*3);
            u8g2.drawPixel(x*3+2,y*3);
            
            u8g2.drawPixel(x*3,y*3+1);
            u8g2.drawPixel(x*3+1,y*3+1);
            u8g2.drawPixel(x*3+2,y*3+1);
            
            u8g2.drawPixel(x*3,y*3+2);
            u8g2.drawPixel(x*3+1,y*3+2);
            u8g2.drawPixel(x*3+2,y*3+2);
            
            }

            // Print each module (UTF-8 \u2588 is a solid block)
            //Serial.print(qrcode_getModule(&qrcode, x, y) ? "\u2588\u2588": "  ");

        }

        //Serial.print("\n");
    }
    
}

uint8_t draw_state = 0;

void draw(void) {
  u8g2_prepare();
 // u8g2_box_frame(draw_state&7);

  qrprint();
}
void setup() {

  
  u8g2.begin();

  Serial.begin(115200);

    // Start time

}


void loop(void) {
  i++;
  // picture loop  
  u8g2.clearBuffer();
  draw();
  u8g2.sendBuffer();
  
  // increase the state
  draw_state++;
  if ( draw_state >= 12*8 )
    draw_state = 0;

  // deley between each page
  delay(100);

}
