// include the library code:
#include <LiquidCrystal.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// LIGHTS
const int redPin = 10;
const int greenPin = 9;
const int bluePin = 6;

// Ratios- Probably wont use
const int max_red = 255;
const int max_green = 255;
const int max_blue = 255;

// Button
const int buttonPin = 14;

int buttonState = 0;
int lightsMode = 0;


void printMode(String(modeName)) {
  lcd.setCursor(0, 1);
  lcd.print(modeName);
}

void setup() {
  // Screen
  lcd.begin(16, 2);
  lcd.print("Welcome to the Lights");
  /*
  delay(2000);
  for(int positionCounter = 0; positionCounter < 5; positionCounter++) {
    lcd.scrollDisplayLeft();
    delay(400);
  }
  delay(1000);
  */

  // LIGHTS
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

  // Button
  pinMode(buttonPin, INPUT);
  
  // Serial
  Serial.begin(115200);
  delay(1000);

  // Display mode 0
  lcd.clear();
  lcd.print("Mode: ");
  lcd.print(lightsMode);
  printMode("Sound Analyzer");
}

void loop() {

  buttonState = digitalRead(buttonPin);

  if (buttonState == HIGH) {
    lightsMode += 1;
    if (lightsMode > 2) {
      lightsMode = 0;
    }
    lcd.clear();
    lcd.print("Mode: ");
    lcd.print(lightsMode);
    delay(1000);
    if (lightsMode == 0) {
      printMode("Sound Analyzer");
    } else if (lightsMode == 1) {
      printMode("White Strobe");
    } else if (lightsMode == 2) {
      printMode("Color Wheel");
    }
  }

  // SOUND ANALYZER MODE 0
  if (lightsMode == 0) {
    if (Serial.read()== ('M')) {
      float redValue = Serial.parseFloat();
      float greenValue = Serial.parseFloat();
      float blueValue = Serial.parseFloat();
  
      if (Serial.read() == '\n') {
        analogWrite(redPin, redValue);
        analogWrite(greenPin, greenValue);
        analogWrite(bluePin, blueValue);
      }
    }
  }
  
  // STROBE MODE 1
  if (lightsMode == 1) {
    // Lights
    analogWrite(greenPin, 255);
    analogWrite(redPin, 255);
    analogWrite(bluePin, 255);
    delay(75);
    analogWrite(greenPin, 0);
    analogWrite(redPin, 0);
    analogWrite(bluePin, 0);
    delay(75);
  }

  // COLOR WHEEL 2
  if (lightsMode == 2) {

    float redValue;
    float blueValue;
    float greenValue;
    
    redValue = 255;
    blueValue = 0;
    greenValue = 0;
    
    for (int i =0; i < 255; i += 1) {
      greenValue += 1;
      redValue -= 1;
      analogWrite(greenPin, greenValue);
      analogWrite(redPin, redValue);
      analogWrite(bluePin, 0);
      delay(10);
    }

    
    redValue = 0;
    blueValue = 0;
    greenValue = 255;
    
    for (int i =0; i < 255; i += 1) {
      blueValue += 1;
      greenValue -= 1;
      analogWrite(bluePin, blueValue);
      analogWrite(greenPin, greenValue);
      analogWrite(redPin, 0);
      delay(10);
    }

    redValue = 0;
    blueValue = 255;
    greenValue = 0;
    
    for (int i =0; i < 255; i += 1) {
      redValue += 1;
      blueValue -= 1;
      analogWrite(redPin, redValue);
      analogWrite(bluePin, blueValue);
      analogWrite(greenPin, 0);
      delay(10);
    }
    
  }

  // COLOR SELECT 3
  if (lightsMode == 3) {
    delay(2000);
    
    float redValue;
    float blueValue;
    float greenValue;
    bool newColors = true;

    while (newColors) {
      redValue = 255;
      blueValue = 0;
      greenValue = 0;
      
      for (int i =0; i < 255; i += 1) {
        greenValue += 1;
        redValue -= 1;
        analogWrite(greenPin, greenValue);
        analogWrite(redPin, redValue);
        analogWrite(bluePin, 0);
        delay(10);
        if (buttonState == HIGH) {
          Serial.print("$$$$$");
          delay(1000);
          newColors = false;
          break;
        }
      }
    }

    while (newColors) {
      redValue = 0;
      blueValue = 0;
      greenValue = 255;
      
      for (int i =0; i < 255; i += 1) {
        blueValue += 1;
        greenValue -= 1;
        analogWrite(bluePin, blueValue);
        analogWrite(greenPin, greenValue);
        analogWrite(redPin, 0);
        delay(10);
        if (buttonState == HIGH) {
          newColors = false;
          break;
        }
      }
    }

    while (newColors) {
      redValue = 0;
      blueValue = 255;
      greenValue = 0;
      
      for (int i =0; i < 255; i += 1) {
        redValue += 1;
        blueValue -= 1;
        analogWrite(redPin, redValue);
        analogWrite(bluePin, blueValue);
        analogWrite(greenPin, 0);
        delay(10);
        if (buttonState == HIGH) {
          newColors = false;
          break;
        }
      }
    }

    analogWrite(redPin, redValue);
    analogWrite(bluePin, blueValue);
    analogWrite(greenPin, greenValue);
    
  }
}
