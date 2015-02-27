#include <Servo.h>  // servo library

// LIGHT SENSOR RELATED VARIABLES
const int sensorPin = 0; // Light sensor in A0
int prevLightLevel = 1023; // global variable for previous light level
int lightLevel; // global variable for light level
const int midLight = 400; // make 200 for normal use, 400 for testing

// SERVO RELATED VARIABLES
Servo servo; // servo control object
int startAngle;
int currentAngle;
int angleHigh;
int angle = 0;
const int angleStep = 10; // number of angles to rotate per loop
const int highestAngle = 180;
const int lowestAngle = 30; // adjusted for backward rotate

// TIME RELATED VARIABLES
unsigned long startTime; // start time
unsigned long elapsedTime; // elapsed time

void setup()
{  
  servo.attach(9); // begin sending control signals to the servo
  servo.write(lowestAngle); // start servo at the lowest angle
  Serial.begin(9600); // for testing purposes
}

void loop()
{
  lightLevel = analogRead(sensorPin); // check if someone is sitting
  // Serial.println(lightLevel); // print light level
  // delay(1000);
  
  if (lightLevel <= midLight && prevLightLevel > midLight) { // just started sitting AKA SETUP
    //Serial.println("GET HERE");
    startTime = millis(); // start the timer
    currentAngle = lowestAngle; // make sure currentAngle is given a number -> lowest angle
    Serial.println("Begin sitting at starting position");
    
  } else if (lightLevel <= midLight && prevLightLevel <= midLight) { // still sitting AKA ACTIVE
    elapsedTime = millis() - startTime; // clock the elapsed time since started sitting
    if (elapsedTime % (1000*5) == 0) { // 5 seconds passed, time to rotate
      // Usually prints three of the below at once, so make sure it rotates less to account for it
      forward_rotate();
      //Serial.println("current angle is " + currentAngle);
    }
    
  } else { // finished sitting
    //Serial.println(currentAngle);
    if (currentAngle != lowestAngle) {
      backward_rotate();
    }
  }
  
  prevLightLevel = lightLevel; // update prev light level to current light level
}

void forward_rotate()
{
  if (angleStep + currentAngle < highestAngle) {
    angleHigh = angleStep + currentAngle;
  } else {
    angleHigh = highestAngle;
  }
  for(angle = currentAngle; angle < angleHigh; angle++)  
  {                                  
    servo.write(angle);               
    delay(30); // used to be 15                 
  } 
  currentAngle = angleHigh;
  Serial.print("Current angle is: ");
  Serial.println(currentAngle);
  //Serial.println("Time elapsed sitting is: " + elapsedTime);
}

void backward_rotate()
{
  for(angle = currentAngle; angle > lowestAngle; angle--)    
  {                                
    servo.write(angle);           
    delay(60); // used to be 15      
  } 
  currentAngle = lowestAngle; // reset currentAngle
  Serial.print("Reset back to starting position");
}


