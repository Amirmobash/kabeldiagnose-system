/*
  Amir Ethernet Continuity Tester
  Author: Amir Mobasheraghdam
  Theme: Orange / White
  Version: 2.0.0

  Clean-room rewrite inspired by community Ethernet continuity tester ideas.
*/

#include "cable_map.h"

struct TestResult {
  bool continuity[CHANNEL_COUNT];
  bool miswire[CHANNEL_COUNT];
  bool shorted[CHANNEL_COUNT];
  int detectedIndex[CHANNEL_COUNT];
  bool pass;
};

TestResult result;

void configurePins() {
  for (int i = 0; i < CHANNEL_COUNT; i++) {
    pinMode(TX_PINS[i], OUTPUT);
    digitalWrite(TX_PINS[i], LOW);

    pinMode(RX_PINS[i], INPUT_PULLUP);
  }
}

void resetResult() {
  result.pass = true;
  for (int i = 0; i < CHANNEL_COUNT; i++) {
    result.continuity[i] = false;
    result.miswire[i] = false;
    result.shorted[i] = false;
    result.detectedIndex[i] = -1;
  }
}

int scanTriggeredInputCount(int &firstDetectedIndex) {
  int count = 0;
  firstDetectedIndex = -1;

  for (int i = 0; i < CHANNEL_COUNT; i++) {
    // INPUT_PULLUP => connected active low
    if (digitalRead(RX_PINS[i]) == LOW) {
      if (firstDetectedIndex == -1) {
        firstDetectedIndex = i;
      }
      count++;
    }
  }

  return count;
}

void testCable() {
  resetResult();

  for (int tx = 0; tx < CHANNEL_COUNT; tx++) {
    // Set all outputs low
    for (int i = 0; i < CHANNEL_COUNT; i++) {
      digitalWrite(TX_PINS[i], LOW);
    }

    delay(2);
    digitalWrite(TX_PINS[tx], HIGH);
    delay(STABILIZE_DELAY_MS);

    int detected = -1;
    int count = scanTriggeredInputCount(detected);

    if (count == 0) {
      result.continuity[tx] = false;
      result.pass = false;
    } else if (count > 1) {
      result.continuity[tx] = false;
      result.shorted[tx] = true;
      result.pass = false;
    } else {
      result.continuity[tx] = true;
      result.detectedIndex[tx] = detected;

      if (detected != EXPECTED_MAP[tx]) {
        result.miswire[tx] = true;
        result.pass = false;
      }
    }

    digitalWrite(TX_PINS[tx], LOW);
    delay(5);
  }
}

void printHeader() {
  Serial.println();
  Serial.println(F("=========================================="));
  Serial.println(F("   AMIR ETHERNET CONTINUITY TESTER"));
  Serial.println(F("   Author: Amir Mobasheraghdam"));
  Serial.println(F("   Theme : Orange / White"));
  Serial.println(F("=========================================="));
}

void printChannelReport(int channel) {
  Serial.print(F("Wire "));
  Serial.print(channel + 1);
  Serial.print(F(": "));

  if (result.shorted[channel]) {
    Serial.println(F("SHORT"));
    return;
  }

  if (!result.continuity[channel]) {
    Serial.println(F("OPEN"));
    return;
  }

  if (result.miswire[channel]) {
    Serial.print(F("MISWIRE -> detected on RX "));
    Serial.println(result.detectedIndex[channel] + 1);
    return;
  }

  Serial.print(F("PASS -> RX "));
  Serial.println(result.detectedIndex[channel] + 1);
}

void printSummary() {
  printHeader();

  for (int i = 0; i < CHANNEL_COUNT; i++) {
    printChannelReport(i);
  }

  Serial.println(F("------------------------------------------"));
  if (result.pass) {
    Serial.println(F("RESULT: PASS"));
  } else {
    Serial.println(F("RESULT: FAIL"));
  }
  Serial.println(F("=========================================="));
}

void setup() {
  Serial.begin(115200);
  configurePins();
  delay(250);
  printHeader();
}

void loop() {
  testCable();
  printSummary();
  delay(LOOP_DELAY_MS);
}
