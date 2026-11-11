// Reflex Tester
// How it works:
//   1. Yellow blinks while waiting. Press the button to start.
//   2. Red to yellow then green lights up one at a time 
//   3. After a random delay, ALL lights go out
//   4. HIT THE BUTTON

// How did ya do?:
//   Green flashes  = success
//   Red flashes    = bad/early
//   Yellow flashes = took too long 
const int redLED = 2;
const int yellowLED = 3;
const int greenLED = 4;
const int buttonPin = 7;

const unsigned long TIMEOUT_MS = 2000;   // time allowed to my slow reaction after GO

void setup() {
  pinMode(redLED, OUTPUT);
  pinMode(yellowLED, OUTPUT);
  pinMode(greenLED, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);     
  Serial.begin(9600);
  randomSeed(analogRead(A0));      
}

bool buttonPressed() {
  return digitalRead(buttonPin) == LOW;
}

void allLEDs(bool on) {
  int level = on ? HIGH : LOW;
  digitalWrite(redLED, level);
  digitalWrite(yellowLED, level);
  digitalWrite(greenLED, level);
}

void waitForRelease() {
  while (buttonPressed()) {}
  delay(100);                
}

// Waits for  milliseconds.
bool pressedDuring(unsigned long ms) {
  unsigned long start = millis();
  while (millis() - start < ms) {
    if (buttonPressed()) return true;
  }
  return false;
}

void flash(int pin, int times, int ms) {
  for (int i = 0; i < times; i++) {
    digitalWrite(pin, HIGH);
    delay(ms);
    digitalWrite(pin, LOW);
    delay(ms);
  }
}

void falseStart() {
  allLEDs(false);
  Serial.println("Too early!");
  flash(redLED, 6, 100);
  waitForRelease();
}

void timeoutResult() {
  Serial.println("Too slow!");
  flash(yellowLED, 4, 250);
}

void loop() {
  allLEDs(false);
  Serial.println("Press the button to start.");

  // Idle: yellow blinks until the button is pressed
  while (!buttonPressed()) {
    if ((millis() / 500) % 2 == 0) {
      digitalWrite(yellowLED, HIGH);
    } else {
      digitalWrite(yellowLED, LOW);
    }
  }
  allLEDs(false);
  waitForRelease();
  delay(500);

// red, then yellow, then green
  digitalWrite(redLED, HIGH);
  if (pressedDuring(1000)) { falseStart(); return; }

  digitalWrite(yellowLED, HIGH);
  if (pressedDuring(1000)) { falseStart(); return; }

  digitalWrite(greenLED, HIGH);
  if (pressedDuring(random(1000, 4001))) { falseStart(); return; }

  allLEDs(false);
  unsigned long goTime = millis();

  while (!buttonPressed()) {
    if (millis() - goTime > TIMEOUT_MS) {
      timeoutResult();
      return;
    }
  }
  unsigned long reaction = millis() - goTime;

  Serial.print("Reaction time: ");
  Serial.print(reaction);
  Serial.println(" ms");
  flash(greenLED, 3, 200);
  waitForRelease();
  delay(1000);
}
