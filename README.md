<img width="1280" height="640" alt="git (1)" src="https://github.com/user-attachments/assets/8920b256-2ba8-4988-b824-5351134eb4bd" />


# 🪞 Periscope — Invisible Morse Code Generator

## Basic Details
### Team Name: Smart Boys

### Team Members
- Team Lead: Aaron Binoy - Saintgits College of Engineering
- Member 2: Abin Sunil - Saintgits College of Engineering

### Project Description
Periscope is a contactless, optical and auditory Morse code telecommunication device. Instead of pressing physical keys, users tap their hands through thin air in front of an ultrasonic sensor. The system processes hand proximity timing, translates short/long proximity breaks into Dots and Dashes, triggers a blinding 12V optical flash relay alongside a piezo audio signal, and outputs decoded text directly to a Serial Interface in real time.

### The Problem (that doesn't exist)
In an age saturated with ultra-fast 5G networks, messaging apps, and instant text delivery, humanity has lost the dramatic tension of waiting several minutes to send a single word. Physical buttons are far too tactile and reliable, while touchscreens lack the thrilling ambiguity of non-contact air gestures. Modern communications completely fail to incorporate high-voltage clicks and blinding optical signals for simple messages.

### The Solution (that nobody asked for)
Periscope fixes modern messaging over-convenience by forcing you to wave your hands like a mad scientist in front of an ultrasonic sensor to spell out messages character-by-character. By measuring hand proximity timing down to the millisecond, it triggers a loud 1-channel relay, flashes a 12V LED light array, sounds an audio buzzer, and decodes your gestures using embedded microcontrollers—delivering a blazing data throughput of roughly 2 words per minute.

---

## Technical Details
### Technologies/Components Used

For Software:
- C / C++ (Arduino Framework)
- C++ Data Structures (`struct` Array Mapping)

For Hardware:
- ESP32 Microcontroller (NodeMCU-32S / ESP-WROOM-32)
- HC-SR04 Ultrasonic Distance Sensor
- 1-Channel Relay Module (5V Coil / Active-HIGH Logic)
- Piezo Passive/Active Buzzer
- 12V LED Strip / Signage Light Array
- 9V / 12V External DC Battery
- Breadboard & Jumper Wires

---

### Implementation

For Software:

# Installation
1. Install [VS Code](https://code.visualstudio.com/) with the [PlatformIO IDE](https://platformio.org/) extension installed (or use the official [Arduino IDE](https://www.arduino.cc/en/software)).
2. Clone this repository to your local machine.


### Run
1.Connect the ESP32 to your PC via a USB cable.

2.Open the Serial Monitor set to 115200 Baud.

3.Place your hand within 10 cm of the ultrasonic sensor:

4.Quick Tap (<300ms): Registers a Dot (.)

4.Hold (≥300ms): Registers a Dash (-)

Pause 1 second: Decodes current symbol pattern into a letter

###Screenshots (Add at least 3)
### <img width="1600" height="852" alt="WhatsApp Image 2026-09-13 at 10 13 12" src="https://github.com/user-attachments/assets/32fd7f27-acba-45c8-a9bb-e45be6889879" />

###PlatformIO / Arduino IDE configuration showing pin mappings and threshold configurations
### <img width="1600" height="852" alt="WhatsApp Image 2026-09-13 at 10 13 12" src="https://github.com/user-attachments/assets/32fd7f27-acba-45c8-a9bb-e45be6889879" />
Demonstration of line-by-line Morse string parsing outputting decoded text

###Diagrams

```mermaid
graph TD
    A[Start Loop: Read HC-SR04 Distance] --> B{Distance <= 10cm?}
    
    B -- YES --> C[Hand Detected]
    C --> D[Buzzer HIGH & Relay HIGH]
    D --> E[Record Start Time]
    
    B -- NO --> F{Was Hand Previously Present?}
    F -- YES --> G[Buzzer LOW & Relay LOW]
    G --> H[Calculate Duration = End - Start]
    
    H --> I{Duration >= 50ms?}
    I -- NO --> J[Discard as Noise]
    I -- YES --> K{Duration < 300ms?}
    
    K -- YES --> L[Append Dot '.']
    K -- NO --> M[Append Dash '-']
    
    L --> N{Idle Pause >= 1000ms?}
    M --> N
    
    N -- YES --> O[Lookup Pattern in morseTable]
    O --> P[Print Decoded Character to Serial]
    P --> Q[Clear currentCode Buffer]
    
    N -- NO --> A
    Q --> A
```



### Schematic & Circuit

USB 5V Power Supply                    9V External Battery
       |                                         |
       +---> [ ESP32 Microcontroller ]           +---> [ Relay COM Terminal ]
                   |      |                                   |
    GPIO 5 (TRIG) -+      +-- VIN (5V) ---> [ Relay VCC ]     | (Switched Path)
    GPIO 18 (ECHO)-+      +-- GPIO 23 ----> [ Relay IN  ]     v
    GPIO 19 -------+---- [ Buzzer (+) ]                [ Relay NO Terminal ]
                          |                                   |
                          |                                   v
                          |                         [ 12V LED Strip (+) ]
                          |                                   |
                          v                                   v
             =======================================================
             COMMON GROUND RAIL  (ESP32 GND = 9V (-) = LED (-))
             =======================================================

### Build Photos

<img width="1200" height="1600" alt="dsdgya" src="https://github.com/user-attachments/assets/aa193baf-a87e-4803-96fa-de7426d00b9a" />
<img width="1200" height="1600" alt="WhatsApp Image 2026-09-13 at 10 23 20" src="https://github.com/user-attachments/assets/170e159c-4da4-488a-bacc-f1eeb0a1b9db" />
<img width="1200" height="1600" alt="bhsdg" src="https://github.com/user-attachments/assets/8e7a7e91-d240-48d1-9375-2395f1fd0640" />

### Project Demo

https://github.com/user-attachments/assets/a8062817-a892-4da8-9685-bf0ada1f5137


### Additional Demos






https://github.com/user-attachments/assets/afbcb81c-4535-4076-bb88-56a7cd4be94d








### Team Contributions
Aaron Binoy: System architecture design, ESP32 microsecond pulse timing implementation, state-machine Morse decoding logic, hardware assembly, and debugging.

Abin Sunil: Hardware circuit wiring, relay-to-12V power path isolation, common ground bus integration, testing, and documentation.

---
Made with ❤️ at TinkerHub Useless Projects 

![Static Badge](https://img.shields.io/badge/TinkerHub-24?color=%23000000&link=https%3A%2F%2Fwww.tinkerhub.org%2F)
![Static Badge](https://img.shields.io/badge/UselessProjects--26-26?link=https%3A%2F%2Ftinkerhub.org%2Fevents%2F1M8ORET9A1%2Fuseless-projects-3.0)



