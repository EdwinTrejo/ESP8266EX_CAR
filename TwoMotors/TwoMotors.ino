#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>

/********* Connections **********/
// Motor connections
#define L298_IN1 9
#define L298_IN2 8
#define L298_IN3 7
#define L298_IN4 6
#define L298_ENA 10
#define L298_ENB 5

/******** Variables ********/
// Direction variables
#define SPEED_CONSTANT 0.2

#define SERIAL_SPEED 115200
#define SERIAL_CONF SERIAL_8E2

const int TIME_DELAY = 30;

const char *FORWARDFORWA = "FORWARDFORWA";
const char *FORWARDRIGHT = "FORWARDRIGHT";
const char *FORWARDLEFTF = "FORWARDLEFTF";

const char *BACKBACKBACK = "BACKBACKBACK";
const char *BACKLEFTBACK = "BACKLEFTBACK";
const char *BACKRIGHTBAC = "BACKRIGHTBAC";

const char *LEFTLEFTLEFT = "LEFTLEFTLEFT";
const char *RIGHTRIGHTRI = "RIGHTRIGHTRI";

const char *STOPSTOPSTOP = "STOPSTOPSTOP";

enum CAR_COMMANDS {
  FORWARD,
  FORWARD_RIGHT,
  RIGHT,
  BACK_RIGHT,
  BACK,
  BACK_LEFT,
  LEFT,
  FORWARD_LEFT,
  STOP
};

#define MSGSIZE 12
byte *instruction;

void setup()
{
  Serial.begin(SERIAL_SPEED, SERIAL_CONF);
  for (int i = 5; i <= 10; i++)
  {
    pinMode(i, OUTPUT);
  }
}

void motor(CAR_COMMANDS dir, int speed);
void get_instruction();

void loop()
{
  delay(TIME_DELAY);
  CAR_COMMANDS current_command = CAR_COMMANDS::STOP;
  motor(current_command, 0);
  delay(TIME_DELAY);
  bool intruction_allowed = false;
  while (!intruction_allowed)
  {
    delay(TIME_DELAY);
    intruction_allowed = true;
    instruction = (byte *)malloc(sizeof(byte) * MSGSIZE);
    get_instruction();

    if      (memcmp(instruction, FORWARDFORWA, MSGSIZE) == 0) current_command = CAR_COMMANDS::FORWARD;
    else if (memcmp(instruction, FORWARDRIGHT, MSGSIZE) == 0) current_command = CAR_COMMANDS::FORWARD_RIGHT;
    else if (memcmp(instruction, FORWARDLEFTF, MSGSIZE) == 0) current_command = CAR_COMMANDS::FORWARD_LEFT;
    else if (memcmp(instruction, BACKBACKBACK, MSGSIZE) == 0) current_command = CAR_COMMANDS::BACK;
    else if (memcmp(instruction, BACKLEFTBACK, MSGSIZE) == 0) current_command = CAR_COMMANDS::BACK_LEFT;
    else if (memcmp(instruction, BACKRIGHTBAC, MSGSIZE) == 0) current_command = CAR_COMMANDS::BACK_RIGHT;
    else if (memcmp(instruction, LEFTLEFTLEFT, MSGSIZE) == 0) current_command = CAR_COMMANDS::LEFT;
    else if (memcmp(instruction, RIGHTRIGHTRI, MSGSIZE) == 0) current_command = CAR_COMMANDS::RIGHT;
    else if (memcmp(instruction, STOPSTOPSTOP, MSGSIZE) == 0) current_command = CAR_COMMANDS::STOP;
    else intruction_allowed = false;

    free(instruction);
  }

  if (current_command == CAR_COMMANDS::LEFT || current_command == CAR_COMMANDS::RIGHT)
  {
    //wait 300 ms
    motor(current_command, 200);
    delay(TIME_DELAY * 10);
  }
  else
  {
    //wait 600 ms
    motor(current_command, 150);
    delay(TIME_DELAY * 20);
  }

  // // 1000ms forward @150 speed
  // //motor(F, 150);
  // delay(1000);
  // // 500ms right @200 speed
  // //motor(R, 200);
  // delay(500);
  // // 1000ms forward @150 speed
  // //motor(F, 150);
  // delay(1000);
  // // 1000ms backward @150 speed
  // //motor(B, 150);
  // delay(1000);
  // // 500ms left @200 speed
  // //motor(L, 200);
  // delay(500);
  // // 1000ms backward @150 speed
  // //motor(B, 150);
  // delay(1000);
}

/*
  speed:
  min: 0, max: 255
*/
void motor(CAR_COMMANDS dir, int speed)
{
  if (dir == CAR_COMMANDS::FORWARD)
  {
    // Left motor forward with full speed
    digitalWrite(L298_IN1, HIGH);
    digitalWrite(L298_IN2, LOW);
    analogWrite(L298_ENA, speed);
    // Right motor forward with full speed
    digitalWrite(L298_IN3, HIGH);
    digitalWrite(L298_IN4, LOW);
    analogWrite(L298_ENB, speed);
  }
  else if (dir == CAR_COMMANDS::FORWARD_RIGHT)
  {
    // Left motor forward with full speed
    digitalWrite(L298_IN1, HIGH);
    digitalWrite(L298_IN2, LOW);
    analogWrite(L298_ENA, speed);
    // Right motor forward with speed*SPEED_CONSTANT
    digitalWrite(L298_IN3, HIGH);
    digitalWrite(L298_IN4, LOW);
    analogWrite(L298_ENB, speed * SPEED_CONSTANT);
  }
  else if (dir == CAR_COMMANDS::RIGHT)
  {
    // Left motor forward with full speed
    digitalWrite(L298_IN1, HIGH);
    digitalWrite(L298_IN2, LOW);
    analogWrite(L298_ENA, speed);
    // Right motor backward with full speed
    digitalWrite(L298_IN3, LOW);
    digitalWrite(L298_IN4, HIGH);
    analogWrite(L298_ENB, speed);
  }
  else if (dir == CAR_COMMANDS::BACK_RIGHT)
  {
    // Left motor backward with full speed
    digitalWrite(L298_IN1, LOW);
    digitalWrite(L298_IN2, HIGH);
    analogWrite(L298_ENA, speed);
    // Right motor backward with speed*SPEED_CONSTANT
    digitalWrite(L298_IN3, LOW);
    digitalWrite(L298_IN4, HIGH);
    analogWrite(L298_ENB, speed * SPEED_CONSTANT);
  }
  else if (dir == CAR_COMMANDS::BACK)
  {
    // Left motor backward with full speed
    digitalWrite(L298_IN1, LOW);
    digitalWrite(L298_IN2, HIGH);
    analogWrite(L298_ENA, speed);
    // Right motor backward with full speed
    digitalWrite(L298_IN3, LOW);
    digitalWrite(L298_IN4, HIGH);
    analogWrite(L298_ENB, speed);
  }
  else if (dir == CAR_COMMANDS::BACK_LEFT)
  {
    // Left motor backward with speed*SPEED_CONSTANT
    digitalWrite(L298_IN1, LOW);
    digitalWrite(L298_IN2, HIGH);
    analogWrite(L298_ENA, speed * SPEED_CONSTANT);
    // Right motor backward with full speed
    digitalWrite(L298_IN3, LOW);
    digitalWrite(L298_IN4, HIGH);
    analogWrite(L298_ENB, speed);
  }
  else if (dir == CAR_COMMANDS::LEFT)
  {
    // Left motor forward with full speed
    digitalWrite(L298_IN1, LOW);
    digitalWrite(L298_IN2, HIGH);
    analogWrite(L298_ENA, speed);
    // Right motor backward with full speed
    digitalWrite(L298_IN3, HIGH);
    digitalWrite(L298_IN4, LOW);
    analogWrite(L298_ENB, speed);
  }
  else if (dir == CAR_COMMANDS::FORWARD_LEFT)
  {
    // Left motor forward with speed*SPEED_CONSTANT
    digitalWrite(L298_IN1, HIGH);
    digitalWrite(L298_IN2, LOW);
    analogWrite(L298_ENA, speed * SPEED_CONSTANT);
    // Right motor forward with full speed
    digitalWrite(L298_IN3, HIGH);
    digitalWrite(L298_IN4, LOW);
    analogWrite(L298_ENB, speed);
  }
  else if (dir == CAR_COMMANDS::STOP)
  {
    // Left motor stop
    analogWrite(L298_ENA, 0);
    // Right motor stop
    analogWrite(L298_ENB, 0);
  }
}

void get_instruction()
{
	while (!Serial.available())
	{
		delay(TIME_DELAY);
	}
	Serial.readBytes(instruction, MSGSIZE);
}
