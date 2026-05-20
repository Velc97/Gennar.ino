#include <arduino.h>
#include <NewPing.h>

#pragma region Parameters

//Ultrasonic sensor
const int trigPin = 10; //Trigger pin
const int echoPin= 9; //Echo pin
#define MAX_DISTANCE 30 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.
NewPing sonarFront(trigPin, echoPin, MAX_DISTANCE); //Sonar class

#pragma endregion Parameters

#pragma region Methods

void sensorsSetup()
{
  Serial.println("Init ultrasonic sensor...");
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  delay(50);
  Serial.println("Init ultrasonic sensor...OK!");
}

//Gets the distance from the ultrasonic sensor in cm. Returns 0 if limit has exceded
float get_distance() {
  delay(40);  //Wait 500ms between pings (about 2 pings/sec). 29ms should be the shortest delay between pings.

  //ping() sends ping, then gets ping time in microseconds (uS).
  return sonarFront.ping() / US_ROUNDTRIP_CM;
}

#pragma endregion Methods