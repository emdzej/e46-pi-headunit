#include <Keyboard.h>

#define COL_COUNT 4
#define ROW_COUNT 10
#define DEBOUNCE_DELAY 10
#define NO_PIN 255

// These are Arduino Leonardo specific pins

const uint8_t COL_PINS[COL_COUNT] = {
  18, 19, 20, 21 // A0 - A3
};
const uint8_t ROW_PINS[COL_COUNT][ROW_COUNT] = {
  // column 0
  {2, 3, 4, 5, 6, 7, NO_PIN, NO_PIN, NO_PIN, NO_PIN},
  // column 1
  {2, 3, 4, 5, 6, 7, NO_PIN, NO_PIN, NO_PIN, NO_PIN},
    // column 2  
  {22 /*A4*/, 23 /*A5*/, 8, NO_PIN, NO_PIN, NO_PIN, NO_PIN, NO_PIN, NO_PIN, NO_PIN},
  // column 3
  {NO_PIN, NO_PIN, NO_PIN, 9, 10, 11, 12, 13, 14, 15 }
};

const char* KEY_CODES[COL_COUNT][ROW_COUNT] = {
  // column 0
  { "SW_INFO", "SW_FM", "SW_MODE_1", "SW_1", "SW_2", "SW_3", NULL, NULL, NULL, NULL },
  // column 1
  { "SW_DISPLAY", "SW_AM", "SW_MODE_2", "SW_4", "SW_5", "SW_6", NULL, NULL, NULL, NULL },
  // colulmn 2
  { "SW_EJECT", "SW_PHONE", "SW_CLOCK", NULL, NULL, NULL, NULL, NULL, NULL, NULL },  
  // column 3
  { NULL, NULL, NULL, "SW_REV", "SW_FWD", "SW_REVERSE", "SW_TONE", "SW_MENU_1", "SW_SEL", "SW_MENU_2"}
};

void setup() {
  uint8_t pin;
  byte col, row;

  Keyboard.begin();

  for(col = 0; col < COL_COUNT; col++) {
    pin = COL_PINS[col];
    if (pin != NO_PIN) {
      pinMode(pin, OUTPUT);
      digitalWrite(pin,  LOW);
    }
  }

  for(col = 0; col < COL_COUNT; col++) {
    for (row = 0; row < ROW_COUNT; row++) {
      pin = ROW_PINS[col][row];
      if (pin != NO_PIN) {
        pinMode(pin, INPUT);
      }
    }
  }
}


int digitalReadDebounced(uint8_t pin) {
  int state;             
  int lastState = LOW; 
  int read = digitalRead(pin);
  unsigned long debounceStart = millis();  

  while ((millis() - debounceStart) < DEBOUNCE_DELAY) {
    read = digitalRead(pin);
    if (read != state) {
      state = read;
    }
  }
  return state;
}

void loop() {
  uint8_t colPin, rowPin;
  byte col, row;
  for(col = 0; col < COL_COUNT; col++) {
    colPin = COL_PINS[col];
    if (colPin == NO_PIN) continue;
    
    digitalWrite(colPin, HIGH);
    for (row = 0; row < ROW_COUNT; row++) {
      rowPin = ROW_PINS[col][row];
      if (rowPin == NO_PIN) continue;
       
      int state = digitalReadDebounced(rowPin);
      if (state == HIGH) {
        const char* key = KEY_CODES[col][row];
        if (key != NULL) {
          Keyboard.print(key);
        }
      }        
    }      
    digitalWrite(colPin, LOW);    
  } 
}
