
/*
 * Author: Charles Garcia
 * Date: Oct. 3, 2018
 * 
 * Reading the matrix:
 *  - The key matrix:
 *    * Rows are pulled down and columns are scanned
 *  - The LED matrix
 *    * Columns are pulled down and rows are scanned
 */

#include <Keyboard.h>
#include <Adafruit_NeoPixel.h>

#define NUMKEYS 16
#define KEYCOLS 4
#define KEYROWS 4
#define PIN 0

#define RED
#define BLUE
#define GREEN
#define YELLOW
#define ORANGE
#define PURPLE


Adafruit_NeoPixel strip = Adafruit_NeoPixel(16, PIN, NEO_GRB + NEO_KHZ800);

// Define your pinouts here
const int       key_Rows[] = {9,8,7,6},
                key_Cols[] = {5,4,3,2},
                led_Cols[] = {A3, A2, A1, A0},
                led_Rows[] = {10, 16, 14, 15};
                
// Define some counters we might use here
uint8_t i, j, k, l;

// Define our ASCII buttons
char macros[NUMKEYS] =   { 177, 98, 99, 177,
                            101, 102, 103, 104,
                            105, 106, 107, 108,
                            109, 110, 111, 112};

// We store our matrix scan here
uint32_t scan_code = 0, scan_prev = 0;

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

void setup() {
  
  // Open serial for debugging messages
  Serial.begin(115200);

  // Initialize our rows and columns
  for(i = 0; i < KEYCOLS; i++ ){
    pinMode(key_Rows[i],INPUT);
    pinMode(key_Cols[i],INPUT_PULLUP);
    pinMode(led_Cols[i], INPUT);
    pinMode(led_Rows[i], INPUT);
  }  
  
  // Initialize our keyboard functionality
  Keyboard.begin();  
  strip.begin();
  strip.show();
  
}

void loop() {


  for(k=0; k<256*5; k++) { // 5 cycles of all colors on wheel
    for(l=0; l< strip.numPixels(); l++) {
      strip.setPixelColor(l, Wheel(((l * 256 / strip.numPixels()) + k) & 255));
    }

  Serial.println("___________________________");
    for(i=0; i<KEYROWS; i++){
    
    // Pull key rows to GND
    pinMode(key_Rows[i], OUTPUT);
    digitalWrite(key_Rows[i], LOW);
    
    // Pull LED columns to GND
    pinMode(led_Cols[i], OUTPUT);
    digitalWrite(led_Cols[i], LOW); 
       
    for(j=0; j<KEYCOLS; j++){

      //Pull up your key columns and scan
      pinMode(key_Cols[j], INPUT_PULLUP);

      // Drive the LED rows HIGH
      pinMode(led_Rows[j], OUTPUT);
      digitalWrite(led_Rows[j], HIGH);

      // Keyboard press
      if(!digitalRead(key_Cols[j])){
        Keyboard.press(macros[i*KEYROWS + j]);
      }
      else{
        Keyboard.release(macros[i*KEYROWS + j]);     
      }

      // Print button state
      Serial.print(" ");
      Serial.print(digitalRead(key_Cols[j]));

      // Disconnect key column
      pinMode(key_Cols[j], INPUT);
    }

    // Disconnect key row
    pinMode(key_Rows[i], INPUT);
    Serial.print("\n");
  }
    strip.show();
  }

}



