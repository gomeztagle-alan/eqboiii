#include <Arduino.h>
#include <U8g2lib.h>
#include <SPI.h>

/*
 * SSD1309 Constructor for 4-wire Hardware SPI
 * F = Full Framebuffer
 * 4W = 4-wire SPI
 * HW_SPI = Hardware SPI
 * Pins: (rotation, cs, dc, [reset])
 */
U8G2_SSD1309_128X64_NONAME0_F_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 17, /* dc=*/ 20, /* reset=*/ 21);

byte audio_bar_height[7]; // sizes for the individual bars
byte audio_bar_peak[7]; // positions for the individual peaks (lines over the bars)



void setup() {
  // 1. Initialize SPI pins for Pico (Optional but recommended for clarity)
  // Default SPI0: SCK=18, MOSI=19
  SPI.setSCK(18);
  SPI.setTX(19); 

  // 2. Start the display
  u8g2.begin();
}

void loop() {
  u8g2.clearBuffer();					
  
  // Set font and draw text
  u8g2.setFont(u8g2_font_ncenB14_tr);	
  u8g2.drawStr(10, 30, "SPI Mode!");         
  
  u8g2.setFont(u8g2_font_6x10_tr);
  u8g2.drawStr(10, 50, "SSD1309 + Pi Pico");
  
  u8g2.sendBuffer();					
  
  delay(1000);
}