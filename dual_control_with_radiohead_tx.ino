#include <SPI.h>
#include <RH_RF95.h>

// Define pins for LoRa (SX1278)
#define LORA_CS 10
#define LORA_RST 9
#define LORA_INT 2

// Frequency for LoRa module (433 MHz for SX1278)
#define LORA_FREQ 433.0 // Frequency in MHz

// Create an instance of the RH_RF95 driver
RH_RF95 rf95(LORA_CS, LORA_INT);

void setup() {
  Serial.begin(9600);
  while (!Serial);

  // Initialize LoRa module
  Serial.println("Initializing LoRa...");

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
  rf95.setTxPower(14, false); // 14 dBm, false disables PA_BOOST
  Serial.println("LoRa initialized successfully!");
}

void loop() {
  // Check if data is available on the serial monitor
  if (Serial.available()) {
    String input = Serial.readStringUntil('\n'); // Read input until newline
    input.trim(); // Remove any leading/trailing whitespace

    // Check if the input command is 
    
    if (input == "led on" || input == "led off" || input == "motor on" || input == "motor off") {
      Serial.print("Sending: ");
      Serial.println(input);

      // Send the command over LoRa
      uint8_t data[input.length() + 1];
      input.getBytes(data, input.length() + 1); // Convert String to byte array
      rf95.send(data, sizeof(data));
      rf95.waitPacketSent();
      Serial.println("Message sent!");

      // Wait for acknowledgment
      uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
      uint8_t len = sizeof(buf);
      bool ackReceived = false;
      unsigned long startTime = millis();

      while (millis() - startTime < 5000) { // Wait up to 5 seconds for ACK
        if (rf95.waitAvailableTimeout(100)) {
          if (rf95.recv(buf, &len)) {
            Serial.print("Acknowledgment received: ");
            Serial.println((char*)buf);

            // Display RSSI value in dBm
            int rssi = rf95.lastRssi();
            Serial.print("RSSI: ");
            Serial.print(rssi);
            Serial.println(" dBm");
            ackReceived = true;
            break;
          }
        }
      }
      if (!ackReceived) {
        Serial.println("Acknowledgment not received!");
      }
    } else {
      Serial.println("Invalid command. Use 'led on', 'led off', 'motor on', or 'motor off'.");
    }
  }
  delay(3);
}