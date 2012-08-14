#include <Servo.h> 

#define servopin 6
#define blinkPin 13

unsigned long currentMillis = 0;
unsigned long prevMillis = 0;
const int clockHandRange = 3600; // range of the display in seconds
unsigned long timer = 3600;
const int setupDelay = 2000;
int stagePins[3] = {3, 4, 5};
int stageThreshold[3] = {600, 300, 60};
const int coinVal = 30;
int blinkStatus = 0;
int blinkMillis = 0;

Servo clockHand;

void setup(){
  //Serial.begin(9600);
  //Serial.println("booting");
  clockHand.attach(servopin);
  clockHand.write(38);
  delay(setupDelay);
  for (int i = 0; i<3; i++){
    pinMode(stagePins[i], OUTPUT);
  }
  attachInterrupt(0, addTime, RISING);
  for (int i=38; i<77; i++){
    clockHand.write(i);
    delay(500);
  }
  //Serial.println("Ready for business");

}

void writeClockHand(int feedback = 1){
    int clockHandPosition = min(map(timer, 0, clockHandRange, 0, 77), 77);
    if (feedback == 1){
      //Serial.print(" clockHandPosition: ");
      //Serial.println(clockHandPosition);
    }
    clockHand.write(clockHandPosition);
}

void addTime(){
  timer = timer + coinVal;
  writeClockHand(0);
}

void doBlink(int on=0){
  if (on == 1){
    digitalWrite(blinkPin, HIGH);
    blinkMillis = currentMillis;
  }
  else if (currentMillis - blinkMillis > 500){
    digitalWrite(blinkPin, LOW);
  }
}
  
  

void loop(){
  currentMillis = millis();
  if ((currentMillis - prevMillis > 1000) && timer > 0){
    timer--;
    //Serial.print("timer: ");
    //Serial.print(timer);
    prevMillis = currentMillis;
    doBlink(1);
    writeClockHand();
  }
  doBlink();
  for (int i = 0; i<3; i++){
      if(timer < stageThreshold[i] && timer > 0){
        digitalWrite(stagePins[i], HIGH);
      }
      else{
        digitalWrite(stagePins[i], LOW);
      }
    }
  }
