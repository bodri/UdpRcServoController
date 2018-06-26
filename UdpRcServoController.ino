#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <Servo.h>

#define MAX_PACKET_SIZE 255

// Soft Ap variables
const char *accesPointSsid = "RcServoControllerBoard";
const char *accesPointPassword = "SomethingDifficult_77";
IPAddress accesPointLocalIp(192, 168, 4, 1);
IPAddress accesPointGateway(192, 168, 4, 1);
IPAddress accesPointSubnet(255, 255, 255, 0);

// UDP Variables
WiFiUDP Udp;
unsigned int localUdpPort = 4473;
char incomingPacket[MAX_PACKET_SIZE];

// General Variables
Servo servo1;
Servo servo2;
const int LEDpin = 2;

void setup() {
  bool initialized = false;
  pinMode(LEDpin, OUTPUT);
  digitalWrite(LEDpin, HIGH);

  WiFi.mode(WIFI_AP);
  if (WiFi.softAP(accesPointSsid, accesPointPassword)) {
    initialized = WiFi.softAPConfig(accesPointLocalIp, accesPointGateway, accesPointSubnet);
  }

  // Setup the UDP port
  Udp.begin(localUdpPort);

  // Attach servos (on Wemos D1 mini pro D1: GPIO5, D2: GPIO4)
  servo1.attach(5);
  servo2.attach(4);

  // Center servos
  servo1.write(90);
  servo2.write(90);

  // Setup has been finished
  if (initialized) {
    digitalWrite(LEDpin, LOW);
  }
}

void loop() {
  if (Udp.parsePacket()) {
    // Receive incoming UDP packets
    int len = Udp.read(incomingPacket, MAX_PACKET_SIZE);
    if (len == 3) {
      int servoIndex = incomingPacket[0];
      int microSeconds = (incomingPacket[1] << 8) + incomingPacket[2];
      if (servoIndex == 1) {
        servo1.write(microSeconds);
      } else if (servoIndex == 2) {
        servo2.write(microSeconds);
      }
    }
  }
}






