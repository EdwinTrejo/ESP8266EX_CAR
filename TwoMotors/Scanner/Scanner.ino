#define __USING_1
#define __USING_2
#define __USING_3
#define __USING_4

#ifdef __USING_1
#if defined(__AVR_ATmega168__) || defined(__AVR_ATmega328P__)
int input_1 = A0; // D14 A0
#elif defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
int input_1 = 2; // D2
#endif
#endif

#ifdef __USING_2
#if defined(__AVR_ATmega168__) || defined(__AVR_ATmega328P__)
int input_2 = A1; // D15 A1
#elif defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
int input_2 = 3; // D3
#endif
#endif

#ifdef __USING_3
#if defined(__AVR_ATmega168__) || defined(__AVR_ATmega328P__)
int input_3 = A2; // D16 A2
#elif defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
int input_3 = 4; // D4
#endif
#endif

#ifdef __USING_4
#if defined(__AVR_ATmega168__) || defined(__AVR_ATmega328P__)
int input_4 = A3; // D17 A3
#elif defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
int input_4 = 5; // D5
#endif
#endif

// the setup routine runs once when you press reset:
void setup()
{
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
#ifdef __USING_1
  pinMode(input_1, INPUT);
#endif

#ifdef __USING_2
  pinMode(input_2, INPUT);
#endif

#ifdef __USING_3
  pinMode(input_3, INPUT);
#endif

#ifdef __USING_4
  pinMode(input_4, INPUT);
#endif
}

// the loop routine runs over and over again forever:
void loop()
{
  // read the input pin:
#ifdef __USING_1
  int read_input_1 = digitalRead(input_1);
  Serial.println("Line 1 :");
  Serial.print(read_input_1);
#endif

#ifdef __USING_2
  int read_input_2 = digitalRead(input_2);
  Serial.println("Line 2 :");
  Serial.print(read_input_2);
#endif

#ifdef __USING_3
  int read_input_3 = digitalRead(input_3);
  Serial.println("Line 3 :");
  Serial.print(read_input_3);
#endif

#ifdef __USING_4
  int read_input_4 = digitalRead(input_4);
  Serial.println("Line 4 :");
  Serial.print(read_input_4);
#endif

  delay(1); // delay in between reads for stability
}