#include <Arduino.h>
#include <TM1637Display.h>
#include <Bounce2.h>

#define CLK 2
#define DIO 3
#define BTN 6
#define START_TIME 600
#define RESET_HOLD_TIME 3000 // Time in ms to hold for reset (3 seconds)

TM1637Display display(CLK, DIO);
Bounce btn = Bounce();

// Variables to track time without blocking the code
unsigned long lastTick = 0;
unsigned long buttonPressedTime = 0;
int timeLeft = START_TIME;
bool isDone = false;

const uint8_t SEG_DONE[] = {
  SEG_B | SEG_C | SEG_D | SEG_E | SEG_G,           // d
  SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F,   // O
  SEG_C | SEG_E | SEG_G,                           // n
  SEG_A | SEG_D | SEG_E | SEG_F | SEG_G            // E
};

void setup() {
  display.setBrightness(0x0f);
  pinMode(BTN, INPUT_PULLUP);
  btn.attach(BTN);
  btn.interval(25);

  
  display.showNumberDec(timeLeft);
}

void loop() {
  btn.update();

  // --- 1. HANDLE BUTTON HOLD TO RESET ---
  if (btn.fell()) { 
    buttonPressedTime = millis(); // Record when the button was first pressed
  }

  if (btn.read() == LOW) { // Button is currently being held
    if (millis() - buttonPressedTime >= RESET_HOLD_TIME) {
      timeLeft = START_TIME;
      isDone = false;
      display.showNumberDec(timeLeft);
      // Optional: Flash the display to show reset happened
    }
  }

  // --- 2. HANDLE THE COUNTDOWN ---
  if (!isDone) {
    // Check if 1 second (1000ms) has passed
    if (millis() - lastTick >= 1000) {
      lastTick = millis();
      timeLeft--;

      if (timeLeft <= 0) {
        timeLeft = 0;
        isDone = true;
      }
      display.showNumberDec(timeLeft);
    }
  } 
  
  // --- 3. HANDLE THE "DONE" BLINKING ---
  else {
    // Simple blinking logic using millis so button remains responsive
    if ((millis() / 500) % 2 == 0) {
      display.setSegments(SEG_DONE);
    } else {
      display.clear();
    }
  }
}