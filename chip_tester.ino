const int clock1Pin = 3;
const int clock2Pin = 6;

const int dataOutPin = 4;
const int dataInPin = 5;

const int failLightPin = 8;
const int passLigthPin = 11;

unsigned long testStep = 0;

unsigned long passCount = 0;

int clockDelayMicroSeconds = 4;

//start with rising edge
bool startEdgeHigh = true;

int bitShift = 1024;
unsigned long testCycles = 100;

unsigned long maxTestSteps = bitShift * testCycles;;

const int testArraySize = 256;
bool mArr[testArraySize];


void setup() {
  Serial.begin(9600);
  pinMode(clock1Pin, OUTPUT);
  pinMode(clock2Pin, OUTPUT);
  pinMode(dataOutPin, OUTPUT);
  pinMode(dataInPin, INPUT);

  pinMode(failLightPin, OUTPUT);
  pinMode(passLigthPin, OUTPUT);

  createTestArray();
  blinkStartLights();
}

void loop() {
  if (testStep < maxTestSteps) {

    int position = testStep % testArraySize;
    int readValue = writeData(testStep, mArr[position]);

    if (testStep > 1023) {
      if (readValue == mArr[position]) {
        passCount++;
      } else {
        testStep = maxTestSteps;
        return;
      }
    }

    //----------------------------------------

    testStep++;
  } else if (testStep == maxTestSteps) {

    Serial.print("Steps: ");
    Serial.print(testStep);
    Serial.print(" Pass Count: ");
    Serial.print(passCount);
    Serial.print(" Diff : ");
    Serial.print(testStep - passCount);


    if (testStep - passCount == bitShift) {
      digitalWrite(passLigthPin, HIGH);
      Serial.print(" PASS!");
    } else {
      digitalWrite(failLightPin, HIGH);
      Serial.print(" FAIL!");
    }


    testStep++;
  }
}
void blinkStartLights() {
  for (int i = 0; i < 3; i++) {
    digitalWrite(failLightPin, HIGH);
    digitalWrite(passLigthPin, HIGH);
    delay(100);
    digitalWrite(failLightPin, LOW);
    digitalWrite(passLigthPin, LOW);
    delay(100);
  }
}
void createTestArray() {
  randomSeed(analogRead(A0));

  Serial.print("\nTest array:\n");
  for (int i = 0; i < testArraySize; i++ ) {
    mArr[i] = random(0, 2);
    Serial.print(mArr[i]);
  }
  Serial.print("\n");
}

int writeData(unsigned long step, int value) {
  bool startEdge = startEdgeHigh;
  bool endEdge = !startEdge;

  int clockPin = (step % 2 == 0) ? clock1Pin : clock2Pin;

  int readValue = -1;

  digitalWrite(clockPin, startEdge);
  digitalWrite(dataOutPin, value);
  digitalWrite(clockPin, endEdge);
  readValue = digitalRead(dataInPin);

  return readValue;
}
