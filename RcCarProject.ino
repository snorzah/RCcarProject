#include <IRremote.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);

#include <Servo.h>
Servo myservo;

int right = 13;
int left = 12;
int serv = 11;
int light = 10;
int pingPin = 9;
int trigPin = 8;
const byte IR_RECEIVE_PIN = 7;
int driveB = 6;
int driveF = 5;
int ledR = 4;
int ledG = 3;
int ledB = 2;

int speed = 0;
int codeData = 0;
bool driveBB = false;
bool driveFB = false;

void setup() {
  // put your setup code here, to run once:
  pinMode(driveB, OUTPUT);
  pinMode(driveF, OUTPUT);
  pinMode(ledR, OUTPUT);
  pinMode(ledG, OUTPUT);
  pinMode(ledB, OUTPUT);
  pinMode(light, OUTPUT);
  pinMode(left, OUTPUT);
  pinMode(right, OUTPUT);
  pinMode(pingPin, INPUT);
  pinMode(trigPin, OUTPUT);
  Serial.begin(9600);
  //digitalWrite(ledR, HIGH);
  digitalWrite(ledG, HIGH);
  digitalWrite(ledB, HIGH);
  IrReceiver.begin(IR_RECEIVE_PIN); //, ENABLE_LED_FEEDBACK);
  lcd.init(); //initialize the lcd
  lcd.backlight(); //turn on the backlight
  myservo.attach(serv);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (IrReceiver.decode()){
    codeData = IrReceiver.decodedIRData.command;
    if ((codeData == 24) && driveFB){
      speed += 15;
      if (speed >= 255){
        speed = 255;
      }
    }
    else if ((codeData == 24) && driveBB){
      speed += 15;
      if (speed >= 120){
        speed = 120;
      }
    }
    else if (codeData == 82){
      speed -= 15;
      if (speed <= 0){
        speed = 0;
      }
    }
    IrReceiver.resume();
  }

  if (codeData == 69){
    driveFB = true;
    driveBB = false;
  }
  else if (codeData == 70){
    driveFB = false;
    driveBB = true;
  }
  else if (codeData == 28){
    driveFB = false;
    driveBB = false;
    speed = 0;
    digitalWrite(driveF, LOW);
    digitalWrite(driveB, LOW);
  }
  else if (codeData == 22 && !speed && (driveFB==false) && (driveBB==false)){
    myservo.write(90);
  }
  else if (codeData == 13 && !speed && (driveFB==false) && (driveBB==false)){
    myservo.write(-90);
  }
  else if (codeData == 71){
    digitalWrite(light, HIGH);
    }
  else if (codeData == 68){
    digitalWrite(light, LOW);
    }
  else if (codeData == 8){
    digitalWrite(left, HIGH);
    digitalWrite(right, LOW);
    }
  else if (codeData == 90){
    digitalWrite(left, LOW);
    digitalWrite(right, HIGH);
    }
  else if (codeData == 25){
    digitalWrite(left, LOW);
    digitalWrite(right, LOW);
    }


  if (driveFB) {
    digitalWrite(ledR, LOW);
    digitalWrite(ledG, LOW);
    digitalWrite(ledB, HIGH);
    analogWrite(driveF, speed);
    digitalWrite(driveB, LOW);
  }
  else if (driveBB) {
    digitalWrite(ledR, LOW);
    digitalWrite(ledG, HIGH);
    digitalWrite(ledB, LOW);
    digitalWrite(driveF, LOW);
    analogWrite(driveB, speed);
  }
  else if (!driveBB && !driveFB && (codeData == 28)){
    digitalWrite(ledR, HIGH);
    digitalWrite(ledG, HIGH);
    digitalWrite(ledB, HIGH);
  }
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Mode: ");
  if (driveFB){
    lcd.print("D");
  }
  else if (driveBB){
    lcd.print("R");
    int cm = ping(pingPin);
    if(cm <= 10){
      speed = 0;
    }
    else if(cm <= 30){
      speed = 50;
    }
  }
  else{
    lcd.print("S");
  }
  lcd.setCursor(0,1);
  lcd.print("Speed: ");
  lcd.print(speed);
  delay(100);
}

int ping(int pingPin) {
  long duration, cm;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(pingPin, HIGH);
  // convert the time into a distance
  cm = duration / 29 / 2;
  return cm ;
}
