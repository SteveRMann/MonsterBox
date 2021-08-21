/*
   Brushed Motor Speed Control
   Using Wemos D1 Mini
*/

// ------ Version history ------
// V1 - Forked from motorControlV4.ino


const int motorPin = D1;
const int analogInPin = A0;               //potentiometer
const int startSwitch = D2;               //Starts the motor
const int stopSwitch = D3;                //Index pin, stops the motor
const long int Minutes = 60000;           //ms per minute
const long int Seconds = 1000;            //ms per second
//int motorValue = 0;
int motorPwm = 256;                       //PWM value for motor on. (lower is slower)
int minimumWait = 30 * Seconds;           //Minimum wait between motor cycles in ms.
int maximumWait = 1 * Minutes;            //Maximum wait between motor cycles in ms.
int holdTime = 3 * Seconds;               //How long in ms to leave the motor on.
//int rssiTime = 15 * Seconds;            //How often to publish RSSI



//--------------- WiFi declarations ---------------
// WiFi declarations
#define SKETCH_NAME "motorControl-door"
#define SKETCH_VERSION "1.0"
#include <ESP8266WiFi.h>        // Not needed if also using the Arduino OTA Library...
#include <Kaywinnet.h>          // WiFi credentials
char macBuffer[24];             // Holds the last three digits of the MAC, in hex.
char hostName[24];              // Holds nodeName + the last three bytes of the MAC address.
char nodeName[] = SKETCH_NAME;  // Give this node a name
//----------- End of WiFi declarations -----------


//--------------- OTA declarations ---------------
#include <ArduinoOTA.h>
//--------------- End of OTA declarations ---------------


//--------------- MQTT declarations ---------------
#include <ESP8266WiFi.h>        // Connect (and reconnect) an ESP8266 to the a WiFi network.
#include <PubSubClient.h>       // connect to a MQTT broker and publish/subscribe messages in topics.
// Declare an object of class WiFiClient, which allows to establish a connection to a specific IP and port
// Declare an object of class PubSubClient, which receives as input of the constructor the previously defined WiFiClient.
// The constructor MUST be unique on the network. (Does it?)
WiFiClient xyzzy;
PubSubClient client(xyzzy);

// Declare strings for the topics. Topics will be created in setup_mqtt().
char statusTopic[20];
char cmndTopic[20];                           // Incoming commands, payload is a command.
// Other topics as needed
//char rssiTopic[20];

const char *mqttServer = MQTT_SERVER;         // Local broker defined in Kaywinnet.h
const int mqttPort = 1883;
//--------------- End of MQTT declarations ---------------


/*
//--------------- dlay declarations ---------------
#include <dlay.h>
dlay holdTimer(750, false);         //How long in ms to hold the motor running
dlay waitTimer(60000, false);       //How long in ms between actions
//dlay rssiTimer(rssiTime, true);     //How often in ms to publish RSSI
//--------------- end dlay declarations ---------------
*/






//=============== setup ===============
void setup() {
  beginSerial();
  setup_wifi();
  start_OTA();
  setup_mqtt();                       // Generate the topics
  mqttConnect();

  //waitTimer.setTime(5000);            //5 seconds initially. Will be random when waitTimer dings.
  //waitTimer.start();                  //After the initial wait time, start the motor.

  pinMode(stopSwitch, INPUT_PULLUP);
  pinMode(startSwitch, INPUT_PULLUP);
}


//=============== loop ===============
void loop() {
  ArduinoOTA.handle();

  //if (waitTimer.ding()) motorOn();     //Turn the motor on
  //if (holdTimer.ding()) motorOff();    //Finished with the motor run hold, turn it off.
  //if (rssiTimer.ding()) publishRssi();

  //Read analog input, pot from 0 to 3.3V
  int potValue = analogRead(analogInPin);
  // map it to the range of the motor:
  motorPwm = map(potValue, 0, 1023, 0, 255);

  if (digitalRead(startSwitch)) {
    Serial.println(F("run"));
    analogWrite(motorPin, motorPwm);      //Get the motor started- past the start switch
    //May need a delay here for the motor to clear the stop switch
  }
  if (digitalRead(stopSwitch)) {          //Wait until the index hits the stop switch
    Serial.println(F("stop"));
    analogWrite(motorPin, 0);             //Stop the motor
  }
}



//=============== Functions ===============
/*
void motorOn() {
  waitTimer.stop();
  analogWrite(motorPin, motorPwm);        //Turn on the motor to motorPwm
  holdTimer.setTime(holdTime);            //Motor on time in ms.
  holdTimer.start();                      //Start the holdTimer
}

void motorOff() {
  holdTimer.stop();
  analogWrite(motorPin, 0);                       //Turn off the motor
  int wait = random(minimumWait, maximumWait);    //Wait between activations (debug)
  waitTimer.setTime(wait);
  waitTimer.start();                              //Start the waitTimer
  Serial.print(F("wait= "));
  Serial.print(wait);
  Serial.println(F(" ms"));
}
*/

/*
  void publishRssi() {
  rssiTimer.stop();
  long myRssi;
  String rssi_str;
  char temp[50];
  myRssi = WiFi.RSSI();                               //RSSI rturns a long int.
  rssi_str = String(myRssi);                          //Convert myRssi to a string
  rssi_str.toCharArray(temp, rssi_str.length() + 1);  //packaging up the data to publish to mqtt
  Serial.print(F("RSSI: "));
  Serial.println(temp);
  client.publish(rssiTopic, temp);
  rssiTimer.start();
  }
*/
