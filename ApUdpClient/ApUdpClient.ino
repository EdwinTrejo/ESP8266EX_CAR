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

/*
   Copyright (c) 2015, Majenko Technologies
   All rights reserved.

   Redistribution and use in source and binary forms, with or without modification,
   are permitted provided that the following conditions are met:

 * * Redistributions of source code must retain the above copyright notice, this
     list of conditions and the following disclaimer.

 * * Redistributions in binary form must reproduce the above copyright notice, this
     list of conditions and the following disclaimer in the documentation and/or
     other materials provided with the distribution.

 * * Neither the name of Majenko Technologies nor the names of its
     contributors may be used to endorse or promote products derived from
     this software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
   ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
   WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
   DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
   ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
   (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
   LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
   ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
   (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/* Create a WiFi access point and provide a web server on it. */

// #define DEBUG
// #define DEBUG_UDP_TX

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <WiFiClient.h>

#ifndef APSSID
#define APSSID "ESPap"
#define APPSK "ForksEverywhere420"
#endif

#define SERIAL_SPEED 115200
#define SERIAL_CONF SerialConfig::SERIAL_8E2

const int TIME_DELAY = 30;
unsigned int localPort = 52864; // local port to listen between 0 to 65,536

/* Set these to your desired credentials. */
const char *ssid = APSSID;
const char *password = APPSK;

// buffers for receiving and sending data
char packetBuffer[UDP_TX_PACKET_MAX_SIZE + 1]; //buffer to hold incoming packet,
#ifdef DEBUG_UDP_TX
char ReplyBuffer[] = "acknowledged\r\n"; // a string to send back
#endif
WiFiUDP Udp;

void setup()
{
  //SerialConfig ser_conf = SerialConfig(SERIAL_8E2);
  delay(TIME_DELAY);
  Serial.begin(SERIAL_SPEED, SERIAL_CONF);
#ifdef DEBUG
  Serial.println();
  Serial.println("Configuring access point...");
#endif
  WiFi.softAP(ssid, password);
#ifdef DEBUG
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
#endif

#ifdef DEBUG
  Serial.printf("UDP server on port %d\n", localPort);
#endif
  Udp.begin(localPort);
}

void loop()
{
  // if there's data available, read a packet
  int packetSize = Udp.parsePacket();
  if (packetSize)
  {
    // LED_BUILTIN 2 on IDE
    // digitalWrite(LED_BUILTIN, LOW);
#ifdef DEBUG
    Serial.printf("Received packet of size %d from %s:%d\r\n (to %s:%d, free heap = %d B)\r\n",
                  packetSize,
                  Udp.remoteIP().toString().c_str(), Udp.remotePort(),
                  Udp.destinationIP().toString().c_str(), Udp.localPort(),
                  ESP.getFreeHeap());
#endif
    // read the packet into packetBufffer
    int n = Udp.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE);
    packetBuffer[n] = 0;
#ifdef DEBUG
    Serial.println("Contents:");
#endif
    Serial.print(packetBuffer);

    // send a reply, to the IP address and port that sent us the packet we received
#ifdef DEBUG_UDP_TX
    Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
    Udp.write(ReplyBuffer);
    Udp.endPacket();
#endif
  }
  delay(TIME_DELAY);
}

/*
  test (shell/netcat):
  --------------------
	  nc -u 192.168.esp.address 8888
*/
