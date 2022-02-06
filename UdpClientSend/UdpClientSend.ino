/*
  UDPSendReceive.pde:
  This sketch receives UDP message strings, prints them to the serial port
  and sends an "acknowledge" string back to the sender

  A Processing sketch is included at the end of file that can be used to send
  and received messages for testing with a computer.

  created 21 Aug 2010
  by Michael Margolis

  This code is in the public domain.

  adapted from Ethernet library examples
*/

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#define PROD

void sendUdpMessage(const char *message);
void reconnectWIFI();
IRAM_ATTR void SendSelectedCommand(int selection);

#ifndef STASSID
#define STASSID "ESPap"
#define STAPSK "ForksEverywhere420"
#endif

// https://raw.githubusercontent.com/RuiSantosdotme/Random-Nerd-Tutorials/master/Projects/ESP8266/ESP8266_Interrupts_Timers.ino

/// This only works if not using the RX3 TX1 or serial
/// https://www.forward.com.au/pfod/ESP8266/GPIOpins/ESP8266_01_pin_magic.html
/// GPIO1 use 330Ω resistor
/// GPIO3 use 330Ω resistor
#define BUTTON_UP_PIN 0
#define BUTTON_DOWN_PIN 2
#define BUTTON_LEFT_PIN 1
#define BUTTON_RIGHT_PIN 3

#define BUTTON_SELECTED_STATE HIGH

boolean button_up_pressed = false;
boolean button_down_pressed = false;
boolean button_left_pressed = false;
boolean button_right_pressed = false;

int _button_up_state;
int _button_down_state;
int _button_left_state;
int _button_right_state;

IRAM_ATTR const char *FORWARDFORWA = "FORWARDFORWA";
IRAM_ATTR const char *FORWARDRIGHT = "FORWARDRIGHT";
IRAM_ATTR const char *FORWARDLEFTF = "FORWARDLEFTF";

IRAM_ATTR const char *BACKBACKBACK = "BACKBACKBACK";
IRAM_ATTR const char *BACKLEFTBACK = "BACKLEFTBACK";
IRAM_ATTR const char *BACKRIGHTBAC = "BACKRIGHTBAC";

IRAM_ATTR const char *LEFTLEFTLEFT = "LEFTLEFTLEFT";
IRAM_ATTR const char *RIGHTRIGHTRI = "RIGHTRIGHTRI";

IRAM_ATTR const char *STOPSTOPSTOP = "STOPSTOPSTOP";

enum CAR_COMMANDS
{
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

volatile unsigned int current_car_command = CAR_COMMANDS::STOP;

const int TIME_DELAY = 30;
unsigned int localPort = 52864; // local port to listen between 0 to 65,536
IPAddress target_ip(192, 168, 4, 1);

WiFiUDP Udp;

void setup()
{
  reconnectWIFI();
  pinMode(BUTTON_UP_PIN, INPUT);
  pinMode(BUTTON_DOWN_PIN, INPUT);
  pinMode(BUTTON_LEFT_PIN, INPUT);
  pinMode(BUTTON_RIGHT_PIN, INPUT);
  Udp.begin(localPort);
}

void loop()
{
  // Check if we are still connected to the WiFi
  if (WiFi.status() != WL_CONNECTED)
    reconnectWIFI();

  delay(TIME_DELAY);

  _button_up_state = digitalRead(BUTTON_UP_PIN);
  _button_down_state = digitalRead(BUTTON_DOWN_PIN);
  _button_left_state = digitalRead(BUTTON_LEFT_PIN);
  _button_right_state = digitalRead(BUTTON_RIGHT_PIN);

  button_up_pressed = (_button_up_state == BUTTON_SELECTED_STATE);
  button_down_pressed = (_button_down_state == BUTTON_SELECTED_STATE);
  button_left_pressed = (_button_left_state == BUTTON_SELECTED_STATE);
  button_right_pressed = (_button_right_state == BUTTON_SELECTED_STATE);

  current_car_command = CAR_COMMANDS::STOP;
  bool command_is_possible = true;

  if (button_up_pressed && button_down_pressed) command_is_possible = false;
  if (button_left_pressed && button_right_pressed) command_is_possible = false;

  if (command_is_possible)
  {
   //FW COMB
    if (button_up_pressed && button_left_pressed)
    {
      current_car_command = CAR_COMMANDS::FORWARD_LEFT;
    }
    else if (button_up_pressed && button_right_pressed)
    {
      current_car_command = CAR_COMMANDS::FORWARD_RIGHT;
    }
    //B COMB
    else if (button_down_pressed && button_left_pressed)
    {
      current_car_command = CAR_COMMANDS::BACK_LEFT;
    }
    else if (button_down_pressed && button_right_pressed)
    {
      current_car_command = CAR_COMMANDS::BACK_RIGHT;
    }
    //FW and B
    else if (button_up_pressed)
    {
      current_car_command = CAR_COMMANDS::FORWARD;
    }
    else if (button_down_pressed)
    {
      current_car_command = CAR_COMMANDS::BACK;
    }
    //LEFT AND RIGHT
    else if (button_left_pressed)
    {
      current_car_command = CAR_COMMANDS::LEFT;
    }
    else if (button_right_pressed)
    {
      current_car_command = CAR_COMMANDS::RIGHT;
    }
    //STOP
    else
    {
      //4 buttons being pressed
      //3 buttons being pressed
      current_car_command = CAR_COMMANDS::STOP;
    }
  }

  SendSelectedCommand(current_car_command);

  delay(TIME_DELAY);
}

IRAM_ATTR void SendSelectedCommand(int selection)
{
  switch (selection)
  {
    case CAR_COMMANDS::FORWARD:
    {
      sendUdpMessage(FORWARDFORWA);
      break;
    }
    case CAR_COMMANDS::FORWARD_RIGHT:
    {
      sendUdpMessage(FORWARDRIGHT);
      break;
    }
    case CAR_COMMANDS::FORWARD_LEFT:
    {
      sendUdpMessage(FORWARDLEFTF);
      break;
    }
    case CAR_COMMANDS::BACK:
    {
      sendUdpMessage(BACKBACKBACK);
      break;
    }
    case CAR_COMMANDS::BACK_RIGHT:
    {
      sendUdpMessage(BACKRIGHTBAC);
      break;
    }
    case CAR_COMMANDS::BACK_LEFT:
    {
      sendUdpMessage(BACKLEFTBACK);
      break;
    }
    case CAR_COMMANDS::RIGHT:
    {
      sendUdpMessage(RIGHTRIGHTRI);
      break;
    }
    case CAR_COMMANDS::LEFT:
    {
      sendUdpMessage(LEFTLEFTLEFT);
      break;
    }
    case CAR_COMMANDS::STOP:
    default:
    {
      sendUdpMessage(STOPSTOPSTOP);
      break;
    }
  }
}

void sendUdpMessage(const char *message)
{
  Udp.beginPacket(target_ip, localPort);
  Udp.write(message);
  Udp.endPacket();
}

void reconnectWIFI()
{
  WiFi.begin(STASSID, STAPSK);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
  }
}
