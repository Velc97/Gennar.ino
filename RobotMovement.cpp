#include <arduino.h>

#pragma region Parameters

//DRV8833 Pinout
const int IN1 = 4;  //left wheels spinning backward
const int IN2 = 5;  //left wheels spinning forward
const int IN3 = 6;  //right wheels spinning backward
const int IN4 = 7;  //right wheels spinning forward

//DRV8833 LEDs
const int led_LF = 2;   //Left front led
const int led_RF = A2;  //Right front led
const int led_LB = 12;  //Left back led
const int led_RB = 11;  //Right back led

float duration, distance;

#define commandStop "st"
#define commandForward "fw"
#define commandBackward "bw"
#define commandTurnRight "tr"
#define commandTurnLeft "tl"
#define commandRotateCW "cw"
#define commandRotateCCW "cc"



#pragma endregion Parameters

enum MoveDirection : byte {
  Stop = 0,
  Forward = 1,
  Backward = 2,
  TurnLeft = 3,
  TurnRight = 4,
  RotateCW = 5,
  RotateCCW = 6
};

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


void move(byte cmd) {

  Serial.print("Moving with: ");
  Serial.println(cmd);

  switch (cmd) {

    case Stop:
      stop();
      break;

    case Forward:
      forward();
      break;

    case Backward:
      backward();
      break;

    case TurnRight:
      turnRight();
      break;

    case TurnLeft:
      turnLeft();
      break;

    case RotateCW:
      rotateCW();
      break;

    case RotateCCW:
      rotateCCW();
      break;

    default:
      Serial.println("Error");
      stop();
      break;
  }
}
