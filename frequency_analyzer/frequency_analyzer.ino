#include <Arduino.h>
#include <U8g2lib.h>
#include <SPI.h>
#include <math.h> // Required for exp()

U8G2_SSD1309_128X64_NONAME0_F_4W_HW_SPI u8g2(U8G2_R0, 17, 20, 21);

float audio_bar_height[7]; 
const int ANALOG_PIN = 26; // Using GP26 (ADC0)
const float SIGMA = 1.2;    // Adjust this to make the "bell" wider or thinner

void setup() {
  u8g2.begin();
  analogReadResolution(10); // Match your 0-1024 logic
  // Setup PWM @ 2.3K
  pinMode(27, OUTPUT);
  tone(27, 2300);

  // Setup Relay signal (depricated)
  pinMode(28, OUTPUT);
}

void loop() {
  u8g2.clearBuffer();
  
  // 1. Read the analog value
  int rawInput = analogRead(ANALOG_PIN); 

  // 2. Map the input to define the CENTER (mu) of the curve across the 7 bars
  // This makes the "hump" move left to right based on the knob/sensor
  float mu = map(rawInput, 0, 1024, 0, 600) / 100.0; 
  
  // 3. Define the max height (Amplitude)
  float amplitude = 50.0; 

  for (int i = 0; i < 7; i++) {
    // 4. Calculate the Gaussian target for this specific bar
    float exponent = -pow((i - mu), 2) / (2 * pow(SIGMA, 2));
    float targetHeight = amplitude * exp(exponent);

    // 5. Smooth transition (Your original logic)
    // We use a small factor (0.2) to keep the movement "silky"
    audio_bar_height[i] = audio_bar_height[i] + ((targetHeight - audio_bar_height[i]) * 0.2);

    // 6. Draw the bar
    u8g2.drawBox(2 + i * 19, 63 - (int)audio_bar_height[i], 10, (int)audio_bar_height[i]);
  }

  u8g2.sendBuffer();
  // Removed the 5-second delay so the animation is actually visible!
  delay(10); 
}