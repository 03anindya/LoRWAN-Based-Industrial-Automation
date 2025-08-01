# 📡 LoRaWAN-Based Industrial IoT Communication System

This repository contains the source code, analysis, and documentation for our project titled **"Implementation of LoRaWAN for Industrial Automation."** The system is designed for secure, long-range, low-power bidirectional wireless communication between a centralized control unit and distributed actuator nodes using the LoRa protocol (433 MHz).

---

## 🧠 Project Overview

This system utilizes LoRaWAN communication, star topology, and bidirectional control to support real-time actuation and feedback in industrial IoT environments. Key innovations include:

- **Bidirectional LoRaWAN Communication** at 433 MHz with acknowledgment
- **Real-time actuator control** using microcontrollers (Arduino UNO)
- **Environmental RSSI performance analysis** over both land and water surfaces
- **Low-power design** suitable for remote deployments
- **Feedback-based execution confirmation** from distributed nodes

---

## ⚙️ Hardware Components

| Component               | Description                                |
|------------------------|--------------------------------------------|
| Arduino UNO R3         | Central control and actuator nodes         |
| SX1278 LoRa Module     | Wireless communication at 433 MHz          |
| LED & DC Motor         | Actuation peripherals                      |
| Whip Antenna (2 dBi)   | Transmission/reception antenna             |
| 9V Battery / USB Power | Power supply                               |

---

## 🔧 Software Setup

### Transmitter (Control Unit)
- Broadcasts commands like `"Motor ON"` or `"LED OFF"`
- Waits for acknowledgment
- Displays status via Serial Monitor

### Receiver (Actuator Node)
- Parses incoming commands
- Triggers motor or LED
- Sends acknowledgment after execution

> Libraries Required:
- `SPI.h`
- `LoRa.h`

---

## 📡 Topology

The system follows a **Star Topology** where:
- One **transmitter (central node)** communicates with multiple **actuator nodes**
- All nodes are independent and can be added/removed dynamically
- Faults in one node do not affect the others

---

## 📊 Experimental Results

Experiments were conducted in the following environments:

- Same floor & different floors (indoors)
- Inside building to outside (urban industrial)
- Above water vs above land (open terrain)

### Key Observations:

- RSSI over water was **~10–20 dB better** than land
- Communication effective up to **212 meters**
- Packet delivery ratio **> 85%** up to 140m
