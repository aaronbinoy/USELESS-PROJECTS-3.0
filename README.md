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
2. Clone this repository to your local machine:
```bash
git clone [https://github.com/Smart-Boys/Periscope-Invisible-Morse-Code-Generator.git](https://github.com/Smart-Boys/Periscope-Invisible-Morse-Code-Generator.git)
cd Periscope-Invisible-Morse-Code-Generator

Open the project folder in your IDE, select ESP32 Dev Module as your board target, and build the project.

Run
Connect your ESP32 board to your PC via a USB cable.

Compile and upload the sketch to your ESP32:

Bash
# PlatformIO CLI Command
pio run --target upload
Open the Serial Monitor set to 115200 Baud rate to view live hand-gesture telemetric logs and decoded text.

Code Implementation
C++
// Pin Definitions
#define TRIG_PIN     5
#define ECHO_PIN     18
#define BUZZER_PIN   19
#define RELAY_PIN    23

// Detection & Timing Thresholds (in milliseconds)
#define MAX_DIST_CM        10    // Hand detection zone (<= 10 cm)
#define DOT_DASH_THRESHOLD 300   // Taps under 300ms = Dot, over 300ms = Dash
#define LETTER_TIMEOUT    1000  // Pause 1 sec to trigger letter decoding
#define WORD_TIMEOUT      2500  // Pause 2.5 sec to insert a space

// State tracking variables
unsigned long signalStartTime = 0;
unsigned long signalEndTime = 0;
bool handPresent = false;
String currentCode = "";

// Morse code lookup structure
struct MorseMapping {
  const char* code;
  char letter;
};

const MorseMapping morseTable[] = {
  {".-", 'A'},   {"-...", 'B'}, {"-.-.", 'C'}, {"-..", 'D'},  {".", 'E'},
  {"..-.", 'F'}, {"--.", 'G'},  {"....", 'H'}, {"..", 'I'},   {".---", 'J'},
  {"-.-", 'K'},  {".-..", 'L'}, {"--", 'M'},   {"-.", 'N'},   {"---", 'O'},
  {".--.", 'P'}, {"--.-", 'Q'}, {".-.", 'R'},  {"...", 'S'},  {"-", 'T'},
  {"..-", 'U'},  {"...-", 'V'}, {".--", 'W'},  {"-..-", 'X'}, {"-.--", 'Y'},
  {"--..", 'Z'}, {".----", '1'},{"..---", '2'},{"...--", '3'},{"....-", '4'},
  {".....", '5'},{"-....", '6'},{"--...", '7'},{"---..", '8'},{"----.", '9'},
  {"-----", '0'}
};
const int morseTableSize = sizeof(morseTable) / sizeof(morseTable[0]);

void setup() {
  Serial.begin(115200);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT);

  digitalWrite(BUZZER_PIN, LOW);
  digitalWrite(RELAY_PIN, LOW);

  Serial.println("\n--- Ultrasonic Morse Decoder Ready ---");
  Serial.println("Tap within 10cm: Quick tap = Dot (.), Hold = Dash (-)");
  Serial.println("Pause 1 sec to complete letter.\n");
}

long getDistanceCM() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 20000);
  if (duration == 0) return 999;
  return duration * 0.0343 / 2;
}

char decodeMorse(String code) {
  for (int i = 0; i < morseTableSize; i++) {
    if (code == morseTable[i].code) {
      return morseTable[i].letter;
    }
  }
  return '?'; // Return '?' if the dot/dash pattern isn't recognized
}

void loop() {
  long distance = getDistanceCM();
  bool isHandDetected = (distance > 0 && distance <= MAX_DIST_CM);

  // 1. Hand newly detected -> Start signal timing
  if (isHandDetected && !handPresent) {
    handPresent = true;
    signalStartTime = millis();
    digitalWrite(BUZZER_PIN, HIGH);
    digitalWrite(RELAY_PIN, HIGH);
  }

  // 2. Hand removed -> Evaluate signal duration (Dot vs Dash)
  if (!isHandDetected && handPresent) {
    handPresent = false;
    signalEndTime = millis();
    digitalWrite(BUZZER_PIN, LOW);
    digitalWrite(RELAY_PIN, LOW);

    unsigned long duration = signalEndTime - signalStartTime;

    if (duration >= 50) { // Ignore minor sensor noise (<50ms)
      if (duration < DOT_DASH_THRESHOLD) {
        currentCode += ".";
        Serial.print(".");
      } else {
        currentCode += "-";
        Serial.print("-");
      }
    }
  }

  // 3. Pause detected -> Decode current symbol sequence into a character
  if (!handPresent && currentCode.length() > 0) {
    unsigned long idleDuration = millis() - signalEndTime;

    if (idleDuration >= LETTER_TIMEOUT) {
      char decodedChar = decodeMorse(currentCode);
      Serial.print(" -> ");
      Serial.println(decodedChar);

      currentCode = ""; // Clear buffer for next letter
    }
  }

  delay(20);
}
Project Documentation
Screenshots
Live Serial Terminal showing real-time dot/dash pulse detection and decoded character output.

User hovering hand within the 10cm threshold to initiate a Morse sequence.

12V LED Strip flashing in sync with the piezo buzzer audio feedback during signal transmission.

Diagrams
System architecture showing signal flow from ultrasonic detection to relay trigger and dictionary lookup.

For Hardware:

Schematic & Circuit
Detailed wiring diagram connecting the ESP32, HC-SR04 ultrasonic sensor, relay module, 12V LED, and buzzer.

Electrical schematic showing high-voltage relay isolated switching path and common ground loops.

Build Photos
Hardware components: ESP32, HC-SR04, 1-channel relay module, piezo buzzer, 12V LED strip, and breadboard.

Breadboard assembly showing common ground junction and relay terminal wiring.

Fully assembled Periscope physical setup ready for optical gesture transmission.

Project Demo
Video
Watch the Periscope Demo Video on YouTube
Demonstrates contactless gesture input, real-time relay switching, 12V light flashing, and Serial Terminal text decoding.

Additional Demos
Project Demo & Code Repository

Team Contributions
Aaron Binoy: Hardware circuit design, high-voltage relay power path integration, and physical system assembly.

Abin Sunil: Embedded C++ development, ultrasonic timing algorithm logic, and Morse code array mapping implementation.

---
Made with ❤️ at TinkerHub Useless Projects 

![Static Badge](https://img.shields.io/badge/TinkerHub-24?color=%23000000&link=https%3A%2F%2Fwww.tinkerhub.org%2F)
![Static Badge](https://img.shields.io/badge/UselessProjects--26-26?link=https%3A%2F%2Ftinkerhub.org%2Fevents%2F1M8ORET9A1%2Fuseless-projects-3.0)



