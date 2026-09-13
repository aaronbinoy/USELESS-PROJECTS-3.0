// Pin Definitions
#define TRIG_PIN     5
#define ECHO_PIN     18
#define BUZZER_PIN   19
#define RELAY_PIN    23

// Detection & Timing Thresholds (in milliseconds)
#define MAX_DIST_CM       10    // Hand detection zone (<= 10 cm)
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