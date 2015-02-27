// library code
#include <LiquidCrystal.h>

// initialize LiquidCrystal library
// pins for the LCD Keypad Shield
LiquidCrystal lcd(8, 13, 9, 4, 5, 6, 7);
// custom character, only lower 5 bits count!
uint8_t custom[8] = {31,17,10,10,0,10,4,0}; // smiley

uint8_t adc_key_pin = 0;
uint16_t adc_key_val[5] = {50, 200, 400, 600, 800};
// none, right, up, down, left, select
int16_t adj[6] = {0, 1, -16, 16, -1, 0};
//uint16_t adc_key_in;  // current key, 0 for no key
//uint16_t adc_key_prev; // previous key, 0 for no key

uint8_t startChar = 0;

// which key was pressed, 0 for no key
uint8_t decodeKey(uint8_t adc_pin) {
  uint8_t key = 0; // assume no key pressed
  uint16_t first, second;
  first = analogRead(adc_pin);
  delay(50);
  second = analogRead(adc_pin);
  //if (abs(second-first) < 10) {
  if ((second ^ first) < 4) { // xor two readings
    key = 0;
    while ((key < 5) && (second > adc_key_val[key])) {
      key += 1;
    }
    key += 1; // 0-4 becomes 1-5
    key = key % 6; // 6 becomes 0
  }
  //lcd.setCursor(0,0);
  //lcd.print(first);
  //lcd.print(":");
  //lcd.setCursor(0,1);
  //lcd.print(second);
  //lcd.print(":");
  return key;
}

void setup() {
  pinMode(13,OUTPUT);
  digitalWrite(13,LOW);
  lcd.begin(16,2);
  lcd.createChar(0,custom);
//  adc_key_in = 0;
//  adc_key_prev = 0;
}

void loop() {
  int i, j, key;
  key = decodeKey(adc_key_pin);
  startChar += adj[key];
  if (key == 5) {
    // select
    startChar = 32;
  }
  for(j=0;j<2;j++) {
    lcd.setCursor(0,j);
    for(i=0;i<16;i++) {
      lcd.write(startChar + j * 16 + i);
    }
  }
  if (key > 0) {
    // indicate keypress delay
    digitalWrite(13,HIGH);
    delay(100);
    digitalWrite(13,LOW);
  }
}
