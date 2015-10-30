/*
 Infrared_Proximity _Object_Existance_Sensor
 Using Infrared Proximity Sensor : to measure distance and existance of our beaker base on Distance mm or cm

 Schematic Crcuit Wiring:
     Red        => 5v 
     Black      => GND
     Yellow     => Analog 0;
 
 And Modified further by Merhawi Zeremariam here : github account
 on 10 Nov 2012.
 */
 #define  sensorpin 0                  // analog pin used to connect the sharp sensor
 const int LED = 11; // Declare the connected LED
 int val = 0;                          // variable to store the values from sensor(initially zero)

 void setup()
{
  Serial.begin(9600);               // starts the serial monitor
}
 
void loop()
{
  val = analogRead(sensorpin);       // reads the value of the sharp sensor
  Serial.println(val);            // prints the value of the sensor to the serial monitor
  analogWrite (LED,val/4);
  delay(100);                    // wait for this much time before printing next value
}
