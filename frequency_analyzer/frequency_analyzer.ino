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



void setup() {
  // 1. Initialize SPI pins for Pico (Optional but recommended for clarity)
  // Default SPI0: SCK=18, MOSI=19
  SPI.setSCK(18);
  SPI.setTX(19); 
  // 2. Start the display
  u8g2.begin();
  u8g2.setColorIndex(1);
  u8g2.setBitmapMode(1);

  audio_bar_height[0]=16;
  audio_bar_height[1]=16;
  audio_bar_height[2]=16;
  audio_bar_height[3]=16;
  audio_bar_height[4]=16;
  audio_bar_height[5]=16;
  audio_bar_height[6]=16;
}

void loop() {
  u8g2.clearBuffer();

  for (int i=0; i<7; i++) { // loop for every fraquency (63Hz, 160Hz, 400Hz, 1kHz, 2.5kHz, 6.25kHz and 16kHz)

    int random_value = random(1024); // calculate random value between 0-1024
    audio_bar_height[i] = audio_bar_height[i] + ((map(random_value, 0, 1024, 0, 53) - audio_bar_height[i]) / 4.0); // update the bar with a new value (slowly)


    u8g2.drawBox(2 + i*19, 53-audio_bar_height[i], 10, audio_bar_height[i]); // draw bar
  }					
  
  // Set font and draw text
  u8g2.setFont(u8g2_font_ncenB14_tr);	
  u8g2.drawStr(  2, 64, "63"); 
  u8g2.drawStr( 19, 64, "160"); 
  u8g2.drawStr( 37, 64, "400"); 
  u8g2.drawStr( 60, 64, "1K"); 
  u8g2.drawStr( 75, 64, "2.5K"); 
  u8g2.drawStr( 95, 64, "6.3K"); 
  u8g2.drawStr(115, 64, "16K");        
  u8g2.sendBuffer();					
  
  delay(5000);
}