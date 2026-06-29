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

//Directions
enum MoveDirection : byte {
  Stop = 0,
  Forward = 1,
  Backward = 2,
  TurnLeft = 3,
  TurnRight = 4,
  RotateCW = 5,
  RotateCCW = 6
};

#pragma endregion Parameters

//Set the output for leds and wheels
inline void setOutputs(
    bool lf, bool rf, bool lb, bool rb,
    bool in1, bool in2, bool in3, bool in4)
{
    // -------- PORTD --------
    // D2 = LF
    // D4 = IN1
    // D5 = IN2
    // D6 = IN3
    // D7 = IN4

    byte portD = PORTD;

    bitWrite(portD, PD2, lf);
    bitWrite(portD, PD4, in1);
    bitWrite(portD, PD5, in2);
    bitWrite(portD, PD6, in3);
    bitWrite(portD, PD7, in4);

    PORTD = portD;

    // -------- PORTB --------
    // D11 = RB
    // D12 = LB

    byte portB = PORTB;

    bitWrite(portB, PB3, rb);
    bitWrite(portB, PB4, lb);

    PORTB = portB;

    // -------- PORTC --------
    // A2 = RF

    byte portC = PORTC;

    bitWrite(portC, PC2, rf);

    PORTC = portC;
}

//Turn off all LEDs
void turnOffAllLEDs() {
  digitalWrite(led_LF, LOW);
  digitalWrite(led_RF, LOW);
  digitalWrite(led_LB, LOW);
  digitalWrite(led_RB, LOW);
}

//Turn right
void turnRight() {
    setOutputs(
        LOW, HIGH, LOW, LOW,
        LOW, HIGH, LOW, LOW);
}

//Turn left
void turnLeft() {
    setOutputs(
        HIGH, LOW, LOW, LOW,
        LOW, LOW, LOW, HIGH);
}


//Go backward
void backward() {
    setOutputs(
        LOW, LOW, HIGH, HIGH,
        HIGH, LOW, HIGH, LOW);
}


//Go forward
void forward() {
    setOutputs(
        HIGH, HIGH, LOW, LOW,
        LOW, HIGH, LOW, HIGH);
}

//pins the robot in clockwise direction.
void rotateCW() {
    setOutputs(
        LOW, LOW, LOW, HIGH,
        LOW, HIGH, HIGH, LOW);
}

//Spins the robot in counter clockwise direction.
void rotateCCW() {
    setOutputs(
        LOW, LOW, HIGH, LOW,
        HIGH, LOW, LOW, HIGH);
}

//Stop the robot
void stop() {
    setOutputs(
        LOW, LOW, LOW, LOW,
        LOW, LOW, LOW, LOW);
}

//Initialize robot pins related to it's movement
void robotSetup() {
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  pinMode(led_LF, OUTPUT);
  pinMode(led_RF, OUTPUT);
  pinMode(led_LB, OUTPUT);
  pinMode(led_RB, OUTPUT);

  stop();
}


//Moves the robot
void move(byte dir) {

  switch (dir) {

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
      stop();
      break;
  }
}
