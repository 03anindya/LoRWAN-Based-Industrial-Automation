#include <SPI.h>
#include <RH_RF95.h>

// Define pins for LoRa (SX1278)
#define LORA_CS 10
#define LORA_RST 9
#define LORA_INT 2

// Frequency for LoRa module (433 MHz for SX1278)
#define LORA_FREQ 433.0 // Frequency in MHz

// Pins for the LED and Motor
#define LED_PIN 4
#define MOTOR_PIN 7

// Create an instance of the RH_RF95 driver
RH_RF95 rf95(LORA_CS, LORA_INT);

void setup() {
  Serial.begin(9600);
  while (!Serial);

  // Initialize pins
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW); // Ensure LED is off initially
  pinMode(MOTOR_PIN, OUTPUT);
  digitalWrite(MOTOR_PIN, LOW); // Ensure Motor is off initially

  // Reset the LoRa module
  pinMode(LORA_RST, OUTPUT);
  digitalWrite(LORA_RST, LOW);
  delay(10);
  digitalWrite(LORA_RST, HIGH);
  delay(10);

  // Initialize the driver
  if (!rf95.init()) {
    Serial.println("RF95 initialization failed!");
    while (1);
  }

  // Set frequency
  if (!rf95.setFrequency(LORA_FREQ)) {
    Serial.println("Setting frequency failed!");
    while (1);
  }
  Serial.print("Frequency set to: ");
  Serial.print(LORA_FREQ);
  Serial.println(" MHz");

  // Set transmission power (2-20 dBm for SX1278)
  rf95.setTxPower(20, false); // 14 dBm, false disables PA_BOOST
  Serial.println("LoRa initialized successfully!");
}

void loop() {
  // Check if a message is received
  uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
  uint8_t len = sizeof(buf);

  if (rf95.waitAvailableTimeout(100)) { // Check for incoming packets
    if (rf95.recv(buf, &len)) {
      String receivedMessage = String((char*)buf);
      receivedMessage.trim(); // Trim any extra whitespace

      Serial.print("Received: ");
      Serial.println(receivedMessage);

      // Act on the received message
      String ackMessage;
      if (receivedMessage == "led on") {
        digitalWrite(LED_PIN, HIGH); // Turn LED on
        Serial.println("LED turned ON");
        ackMessage = "LED is ON";
      } else if (receivedMessage == "led off") {
        digitalWrite(LED_PIN, LOW); // Turn LED off
        Serial.println("LED turned OFF");
        ackMessage = "LED is OFF";
      } else if (receivedMessage == "motor on") {
        digitalWrite(MOTOR_PIN, HIGH); // Turn Motor on
        Serial.println("MOTOR turned ON");
        ackMessage = "MOTOR is ON";
      } else if (receivedMessage == "motor off") {
        digitalWrite(MOTOR_PIN, LOW); // Turn Motor off
        Serial.println("MOTOR turned OFF");
        ackMessage = "MOTOR is OFF";
      } else {
        Serial.println("Unknown command received");
        ackMessage = "Unknown command";
      }

      // Send acknowledgment
      uint8_t ackData[ackMessage.length() + 1];
      ackMessage.getBytes(ackData, ackMessage.length() + 1);
      rf95.send(ackData, sizeof(ackData));
      rf95.waitPacketSent();
      Serial.println("Acknowledgment sent: " + ackMessage);
    }
  }
  delay(3);
}
