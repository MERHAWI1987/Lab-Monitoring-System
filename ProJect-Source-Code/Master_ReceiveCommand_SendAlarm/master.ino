/*The Manager
 *Modified by: Merhawi Zeremariam
* Sketch that request sensors status or values and based on the set up controls the *actuator, and also warns the client if the system is in Danger
*This sketch, for the Arduino GSM shield, waits for a SMS message
 *and displays it through the Serial port.

 *Circuit:
 *GSM shield attached to and Arduino
 * SIM card that can receive SMS messages

  *created 25 Feb 2012
 *by Javier Zorzano / TD
 *http://www.arduino.cc/en/Tutorial/GSMExamplesReceiveSMS

*/

//Library
#include <Wire.h>

// include the GSM library
#include <GSM.h>

// PIN Number for the SIM
#define PINNUMBER ""
#define ledPin 13


#define ledRed 10
#define ledGreen 4
#define ledBlue 11

// initialize the library instances
GSM gsmAccess;
GSM_SMS sms;

// Array to hold the number a SMS is retreived from
char senderNumber[20];
const String myPhone = "+99999999999";
int delayval = 500; // delay for half a second
int Sensors[] = {0,0,0,0};
int maximumRange = 28;


void setup() 
{
   // join I2C bus (address optional for master)
    Wire.begin();                   
    
  // initialize serial communications and wait for port to open:
  Serial.begin(9600);

  //initialize System ON/Off
  pinMode(ledPin,OUTPUT);

  //initialize my Motor presenting LED
  pinMode(ledRed,OUTPUT);
  pinMode(ledGreen,OUTPUT);
  pinMode(ledBlue,OUTPUT);
  
  //Wait Setrial Port to connect
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  //Wait for text to start your device
  Serial.println("System Start  Mode");
  Serial.println("Wait for the GSM to connect to the modem");
  
  //by Default the test Object is OFF
  digitalWrite(ledPin , LOW);
  Serial.println("The System is OFF by Default");
  
  // connection state for the GSM system
  boolean notConnected = true;

  // Start GSM connection or Network Connection
  while (notConnected) {
    if (gsmAccess.begin(PINNUMBER) == GSM_READY) {
      notConnected = false;
    } else {
      Serial.println("Not connected");
      delay(1000);
    }
  }
  Serial.println("Turn the led ON to check the GSM is initialize");
  digitalWrite(ledPin , HIGH);
  delay(300);
  digitalWrite(ledPin , LOW);	
  Serial.println("GSM initialized");
  Serial.println("First State Red LED");
  digitalWrite(ledRed, !HIGH);
  digitalWrite(ledGreen,! LOW);
  digitalWrite(ledBlue, !LOW);
  }

void loop() 
{
  char c, d;
  String msg = "";
  char remoteNum[] = "99999999999";  // telephone number to send sms
  
   // First State After GSM is initialized.
   
  //readSerial(remoteNum);
  Serial.println("\nmy boss Emergency Contact ");
  Serial.print("         ");
  Serial.println(remoteNum);
  // sms text to send to alarm the client
  Serial.println("\nThe alarm Text message");
  char txtMsg[]= "BOSS save me I am on Fire";
  Serial.println(txtMsg);
  
    Serial.println("\nRequest 4 Bytes from Slave node #2");
    Wire.requestFrom(2, 4);    // request 4 bytes from slave device #2
    for(int i=0;i<4;i++)
    { 
    int d = Wire.read();  // receive a byte as character
    Sensors[i]=d;
    delay(300);
    Serial.print('\t');    
     }
      Serial.println("\nThese are your Sensors reading");
      Serial.print(Sensors[0]);
      
      Serial.print('\t'); 

      Serial.print(Sensors[1]);
      Serial.print('\t'); 

      
      Serial.print(Sensors[2]);
      Serial.print('\t');
      Serial.print(Sensors[3]);
       if((Sensors[0]>= 4)  && (Sensors[0]<= 10))
          {
            if((Sensors[1]>=15) && ( Sensors[1]<= 20))
               {
                  Serial.println("\nFull GREEN");
                  digitalWrite(ledRed, !LOW);
                  digitalWrite(ledGreen, !HIGH);
                  digitalWrite(ledBlue, !LOW);
                }
                  else
		{
                  	Serial.println("\n I am Filling it BLUE ");
                  	digitalWrite(ledRed, !LOW);
                  	digitalWrite(ledGreen, !LOW);
                  	digitalWrite(ledBlue, !HIGH);
		}
          }
          if(!((Sensors[0]>= 4)  && (Sensors[0]<= 10)))
	{
		//No Breakes   
      		Serial.println("\nNO Beaker");  
  	}  
      
   //Check Condition for my Temperature if excessed the setpoint
   //Serial.println("Check for Temperature sensor if exceed the setpoint");
      if( Sensors[2]> 27)
      {
        delay(100);
        // send the message
        sms.beginSMS(remoteNum);
        sms.print(txtMsg);
        sms.endSMS();
        Serial.println("The message will be send");
        Serial.println("\nCOMPLETE!\n");
      }
      else
	{
        		Serial.println("No message sent");
        		delay(100);
      	} 
  // If there are any SMSs available()
  //Serial.println("wait for text"); 
  if (sms.available() ) {
    Serial.println("Waiting for messages");
    Serial.println("Message received from:");

    // Get remote number
    sms.remoteNumber(senderNumber, 20);
    Serial.println("       ");


    // An example of message disposal
    // Any messages starting with # should be discarded
    if (sms.peek() == '#') 
     {
      Serial.println("Discarded SMS");
      sms.flush();
     }

    Serial.println(" check the sender Number, if  it matches , My BOSS Number ");
   if((myPhone == senderNumber))
   {
     Serial.print('\n');
     Serial.print("Access Granted");
    // Read message bytes and print them
     Serial.println("Read my Command message Bytes and Print them ");
    while (c = sms.read()) 
    {
      Serial.print(c);
      msg += c;
    }
    Serial.print('\n');
    Serial.print(msg);
    Serial.print('\n');
    Serial.println("Tell me Out do");
      if( (msg == "OFF"))
    {
      digitalWrite(ledPin, LOW);
      Serial.println("\nSystem OFF");
    }
      if((msg == "ON"))
      {
        digitalWrite(ledPin, HIGH);
        Serial.println("System ON");
        }	
    }
    Serial.println("\nEND OF MESSAGE");
    // Delete message from modem memory
   Serial.print("\nDELETE  MESSAGE from the modem memory");
    sms.flush();
    Serial.println("MESSAGE DELETED");
  }
  delay(100);
}
