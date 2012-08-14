// Code written by Piet De Vaere for an electricty timer. More info about the project can be found here: http://blog.devae.re/?p=19
// Feel free do do whatever you want with the code, but I would appreciate it if you left these two lines in place
#include <Servo.h> 

#define servopin 6  // Servo attached to pin 6
#define blinkPin 13 // blinkinglight attached to pin 13


unsigned long currentMillis = 0;  // these are for timer purposes
unsigned long prevMillis = 0;
unsigned long blinkMillis = 0;

// All time related variables declared below are in seconds 
const int clockHandRange = 3600; // range of the display (time)
const int clockHandMaxAngle = 77; // range of the display (angle)
unsigned long timer = 600; // start value of the timer
const int setupDelay = 2000; 
int stagePins[3] = {3, 4, 5};  // pins the various allarms are attached to
int stageThreshold[3] = {600, 300, 60}; // treshold values for these alarms
// When the timer goes below these values the corresponding pins will go high
const int coinVal = 140; // The amount of time to add to the timer when a coin is droped in
int blinkStatus = 0;

Servo clockHand;

void setup(){
  Serial.begin(9600);
  Serial.println("booting");
  clockHand.attach(servopin);
  clockHand.write(clockHandMaxAngle/2);  // move the servo to the middle of the 
  delay(setupDelay);
  for (int i = 0; i<3; i++){
    pinMode(stagePins[i], OUTPUT); // set the alarm pins to outputs
  }
  attachInterrupt(0, addTime, RISING);
  for (int i=38; i>13; i--){
    clockHand.write(i);  // slowly move the servo to the correct position
    delay(500);
  }
  Serial.println("Ready for business");

}

void writeClockHand(int feedback = 1, int boot = 0){
    int clockHandPosition = min(map(timer, 0, clockHandRange, 0, clockHandMaxAngle), clockHandMaxAngle); // Calculate where the clockhand should be
    if (feedback == 1){ // give some feedback
      Serial.print(" clockHandPosition: ");
      Serial.println(clockHandPosition);
    }
    clockHand.write(clockHandPosition); // move the clockhand there
}

void addTime(){
  timer = timer + coinVal;  // add some seconds to the timer
  writeClockHand(0); // and move the clockhand
}

void doBlink(int on=0){ 
  if (on == 1){ // if we have to turn the led on
    digitalWrite(blinkPin, HIGH); // turn it on
    blinkMillis = currentMillis; // set the time this has happend
  }
  else if (currentMillis - blinkMillis > 500){ //otherwise, if half a second has passed
    digitalWrite(blinkPin, LOW); // turn it back of
  }
}
  
  

void loop(){
  currentMillis = millis();
  if ((currentMillis - prevMillis > 1000) && timer > 0){ // if a second has passed and the timer is not yet at zero
    timer--; // substract a second
    Serial.print("timer: ");
    Serial.print(timer);
    prevMillis = currentMillis; // set the time this has happend
    doBlink(1); // turn on the led
    writeClockHand(); // move the clockhand
  }
  doBlink(); // check if it is time to turn the blinkinglight of
  for (int i = 0; i<3; i++){ // check if we shoudl write the alarmpins high or low
      if(timer < stageThreshold[i]){ 
        digitalWrite(stagePins[i], HIGH);
      }
      else{
        digitalWrite(stagePins[i], LOW);
      }
    }
  }
