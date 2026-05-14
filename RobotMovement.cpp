#include <arduino.h>

#pragma region Parameters

//DRV8833 Pinout
const int IN1 = 4; //left wheels spinning backward
const int IN2 = 5; //left wheels spinning forward
const int IN3 = 6; //right wheels spinning backward
const int IN4 = 7; //right wheels spinning forward

//DRV8833 LEDs
const int led_LF = 2;   //Left front led
const int led_RF = A2;  //Right front led
const int led_LB = 12;  //Left back led
const int led_RB = 11;  //Right back led

float duration, distance;

//Robot movement commands
#define commandStop "st"
#define commandForward "fw"
#define commandBackward "bw"
#define commandTurnRight "tr"
#define commandTurnLeft "tl"
#define commandRotateCW "cw"
#define commandRotateCCW "cc"

//Bluetooth response string
#define bluetoothTurnOff "+DISC:SUCCESS"

#pragma endregion Parameters

//Turn off all LEDs
void turnOffAllLEDs() {
  digitalWrite(led_LF, LOW);
  digitalWrite(led_RF, LOW);
  digitalWrite(led_LB, LOW);
  digitalWrite(led_RB, LOW);
}

//Turn right
void turnRight() {
  digitalWrite(led_LF, LOW);
  digitalWrite(led_RF, HIGH);
  digitalWrite(led_LB, LOW);
  digitalWrite(led_RB, LOW);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

//Turn left
void turnLeft() {
  digitalWrite(led_LF, HIGH);
  digitalWrite(led_RF, LOW);
  digitalWrite(led_LB, LOW);
  digitalWrite(led_RB, LOW);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}


//Go backward
void backward() {
  digitalWrite(led_LF, LOW);
  digitalWrite(led_RF, LOW);
  digitalWrite(led_LB, HIGH);
  digitalWrite(led_RB, HIGH);

  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

//Go forward
void forward() {
  digitalWrite(led_LF, HIGH);
  digitalWrite(led_RF, HIGH);
  digitalWrite(led_LB, LOW);
  digitalWrite(led_RB, LOW);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

//pins the robot in clockwise direction.
void rotateCW() {
  digitalWrite(led_LF, LOW);
  digitalWrite(led_RF, LOW);
  digitalWrite(led_LB, LOW);
  digitalWrite(led_RB, HIGH);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

//Spins the robot in counter clockwise direction.
void rotateCCW() {
  digitalWrite(led_LF, LOW);
  digitalWrite(led_RF, LOW);
  digitalWrite(led_LB, HIGH);
  digitalWrite(led_RB, LOW);

  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

//Stop the robot
void stop() {
  turnOffAllLEDs();

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}


//Initialize robot pins related to it's movement
void robotSetup() {
  Serial.println("Init DRV8833 driver");
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  Serial.println("Init DRV8833 driver done");

  Serial.println("Init LED");
  pinMode(led_LF, OUTPUT);
  pinMode(led_RF, OUTPUT);
  pinMode(led_LB, OUTPUT);
  pinMode(led_RB, OUTPUT);
  Serial.println("Init LED done");

  Serial.println("Init Car functions");
  stop();
  Serial.println("Init Car functions done"); 
}

//Elaborates the received string input as a command
void elaborateCommand(String inputString) {

    inputString.trim();
    Serial.println("Received command from serial:" + inputString);

    const char* inputCharArray = inputString.c_str();

    if (strcmp(inputCharArray, commandStop) == 0 || strcmp(inputCharArray, bluetoothTurnOff) == 0) {
      stop();
    } else if (strcmp(inputCharArray, commandForward) == 0) {
      forward();
    } else if (strcmp(inputCharArray, commandBackward) == 0) {
      backward();
    } else if (strcmp(inputCharArray, commandTurnRight) == 0) {
      turnRight();
    } else if (strcmp(inputCharArray, commandTurnLeft) == 0) {
      turnLeft();
    } else if (strcmp(inputCharArray, commandRotateCW) == 0) {
      rotateCW();
    } else if (strcmp(inputCharArray, commandRotateCCW) == 0) {
      rotateCCW();
    }
}

