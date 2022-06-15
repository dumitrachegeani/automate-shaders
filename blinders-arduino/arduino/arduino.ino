#include <LiquidCrystal_I2C.h>
#include <stdlib.h>
LiquidCrystal_I2C lcd(0x27, 16, 2); // I2C address 0x27, 16 column and 2 rows

String readString;

#define A1A 7
#define A1B 6
unsigned long startMillis;  //some global variables available anywhere in the program
unsigned long currentMillis;
const unsigned long period = 1000;  //the value is a number of milliseconds

void setup() {
  Serial.begin(115200);
  pinMode(A1A, OUTPUT);
  pinMode(A1B, OUTPUT);
  lcd.begin(); // initialize the lcd
  lcd.backlight();
  
  lcd.setCursor(1, 0);         // move cursor to   (0, 0)
  lcd.print("Automatic");        // print message at (0, 0)
  lcd.setCursor(1, 1);         // move cursor to   (2, 1)
  lcd.print("Blinders"); // print message at (2, 1) 
  startMillis = millis();
}

void loop() {
   
    
  while (Serial.available()) {
    delay(5);  //delay to allow buffer to fill 
    if (Serial.available() >0) {
      char c = Serial.read();  //gets one byte from serial buffer
      readString += c; //makes the string readString
    } 
  }
  
  
  if (readString.length() >0) {

      Serial.println(readString);
      String lift = String("lifting");
      String down = String("putting down");
      char Buf[50];
      readString.toCharArray(Buf, 50);
      Serial.println(Buf);
      if (strncmp(Buf, "lifting", 5) == 0) {
        lcd.clear();
        lcd.setCursor(3,0);
        lcd.print(readString);
        lcd.setCursor(3,1);
        lcd.print("blinders");
        motorRight();
        delay(10000);
        motorStop();
      } 
      else if (strncmp(Buf, "putting", 5) == 0){
        lcd.clear();
        lcd.setCursor(1,0);
        lcd.print(readString);
        lcd.setCursor(3,1);
        lcd.print("blinders");
        motorLeft();
        delay(10000);
        motorStop();
      } else {
        motorStop();
        lcd.clear();
        lcd.print("stop");
      }
      readString="";
  } 
}


void motorRight() {
  digitalWrite(A1A, HIGH);
  digitalWrite(A1B, LOW);
  
}

void motorStop() {
  digitalWrite(A1A, LOW);
  digitalWrite(A1B, LOW);
  lcd.setCursor(1, 0);         // move cursor to   (0, 0)
  lcd.print("Automatic");        // print message at (0, 0)
  lcd.setCursor(1, 1);         // move cursor to   (2, 1)
  lcd.print("Blinders"); // print message at (2, 1)  
}

void motorLeft(){
   digitalWrite(A1A, LOW);
   digitalWrite(A1B, HIGH);
}
