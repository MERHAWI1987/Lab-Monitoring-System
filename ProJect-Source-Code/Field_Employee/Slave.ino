//Modified By : Merhawi Zeremariam
//The employee that takes Reading 
//Send Data
// Wire Slave Sender
#include <Wire.h>

 //Local Variables
#define Temp36  A0
const int trigPin_0 = 7;
const int echoPin_0 = 8;
const int trigPin_1 = 4;
const int echoPin_1 = 5;
const int buttonPin = 2;

//Global variables
int Sensors[]={0,0,0,0}; 

int currentButton = 1 ;   // Current Button State
int lastButton = 1;       // last Button State
boolean buttonState = 0;
int maximumRange = 50;    // Maximum range needed
int minimumRange = 0;     // Minimum range needed
long duration, distance, beakerPosition, beakerLevel ; // Duration used to calculate distance

void setup()
{
  Serial.begin(9600);
  ultrasonicSensorInit(trigPin_0, echoPin_0);
  ultrasonicSensorInit(trigPin_1, echoPin_1);
  pinMode(buttonPin,INPUT);
  Wire.begin(2);                      // join i2c bus with address #2
  Wire.onRequest(requestEvent);       // register event
}

void loop()
{
 
  beakerPosition = distanceSensor(trigPin_0,echoPin_0);
  beakerLevel = distanceSensor(trigPin_1,echoPin_1);
  Sensors[0]= beakerPosition;
  Sensors[1]= beakerLevel;
   
  //getting the Temperature reading from Temp Lm36
  int reading = analogRead (Temp36);
  float volts = AnalogToVolts(reading);  //Function call
  float temperatureC = VoltsToTemperatureC(volts);
  temperatureC = VoltsToTemperatureC(volts); 
  Sensors[2]= temperatureC;
  Sensors[3] = 0;
  currentButton = debounce(lastButton);
  if (lastButton == LOW && currentButton == HIGH)
    {
      buttonState = !buttonState;
    }
  lastButton = currentButton;  
  Sensors[3] = buttonState;
  Serial.print(Sensors[0]);
  Serial.print('\t');
  Serial.print(Sensors[1]);
  Serial.print('\t');
  Serial.print(Sensors[2]);
  Serial.print('\t');
  Serial.print(Sensors[3]);
  Serial.print('\n');
  delay(300);
}

// function that executes whenever data is requested by master
// this function is registered as an event, see setup()
void requestEvent()
{
//  for(int i=0;i<4;i++)
//  {
  uint8_t Buffer[4];
  Buffer[0]=Sensors[0];
  Buffer[1]=Sensors[1];
  Buffer[2]=Sensors[2];
  Buffer[3]=Sensors[3];
  
  Wire.write(Buffer,4); 
   
}

long distanceSensor(int trigPin,int echoPin)
{
digitalWrite(trigPin, LOW);
delayMicroseconds(2);
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);
duration = pulseIn(echoPin, HIGH);
distance = (duration/58.2);
return distance;
}
// Function Definition
float AnalogToVolts(int reading)
{
  float volts;
  volts = reading/1023.0 * 5.0;       //Perform conversion
  return volts;                       //Return result
}

// Function Definition
float VoltsToTemperatureC(float volts)
{
  float temperatureC;
  temperatureC = (volts - 0.5) * 100 ;            //converting from 10 mv per degree wit 500 mV offset
                                                  //to degrees ((voltage - 500mV) times 100)
  return temperatureC;                              //Return result
}

//to accurate debounce response of Pushbutton by passing the last state to compare
boolean debounce(int last)
{
 boolean current = digitalRead(buttonPin); //This variable read the current state of the pushbutton state value
 if(last!= current)
 {
 delay(6);
 current = digitalRead(buttonPin); //This current variable will read the current state of the pushbutton state value
 }
 return current;
}

void ultrasonicSensorInit(int trigPin, int echoPin)
{
  pinMode(trigPin,OUTPUT);
  pinMode(echoPin, INPUT); 
}
