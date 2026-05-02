#include "cable_map.h"

struct Testergebnis {
  bool verbindung[CHANNEL_COUNT];
  bool falschVerdrahtet[CHANNEL_COUNT];
  bool kurzschluss[CHANNEL_COUNT];
  int erkannterPin[CHANNEL_COUNT];
  bool bestanden;
};

Testergebnis ergebnis;

void pinsEinrichten() {
  for (int i = 0; i < CHANNEL_COUNT; i++) {
    pinMode(TX_PINS[i], OUTPUT);
    digitalWrite(TX_PINS[i], LOW);
    pinMode(RX_PINS[i], INPUT_PULLUP);
  }
}

void ergebnisZuruecksetzen() {
  ergebnis.bestanden = true;

  for (int i = 0; i < CHANNEL_COUNT; i++) {
    ergebnis.verbindung[i] = false;
    ergebnis.falschVerdrahtet[i] = false;
    ergebnis.kurzschluss[i] = false;
    ergebnis.erkannterPin[i] = -1;
  }
}

int erkannteEingaengeZaehlen(int &ersterErkannterPin) {
  int anzahl = 0;
  ersterErkannterPin = -1;

  for (int i = 0; i < CHANNEL_COUNT; i++) {
    if (digitalRead(RX_PINS[i]) == LOW) {
      if (ersterErkannterPin == -1) {
        ersterErkannterPin = i;
      }
      anzahl++;
    }
  }

  return anzahl;
}

void kabelTesten() {
  ergebnisZuruecksetzen();

  for (int tx = 0; tx < CHANNEL_COUNT; tx++) {
    for (int i = 0; i < CHANNEL_COUNT; i++) {
      digitalWrite(TX_PINS[i], LOW);
    }

    delay(2);
    digitalWrite(TX_PINS[tx], HIGH);
    delay(STABILIZE_DELAY_MS);

    int erkannt = -1;
    int anzahl = erkannteEingaengeZaehlen(erkannt);

    if (anzahl == 0) {
      ergebnis.verbindung[tx] = false;
      ergebnis.bestanden = false;
    } else if (anzahl > 1) {
      ergebnis.verbindung[tx] = false;
      ergebnis.kurzschluss[tx] = true;
      ergebnis.bestanden = false;
    } else {
      ergebnis.verbindung[tx] = true;
      ergebnis.erkannterPin[tx] = erkannt;

      if (erkannt != EXPECTED_MAP[tx]) {
        ergebnis.falschVerdrahtet[tx] = true;
        ergebnis.bestanden = false;
      }
    }

    digitalWrite(TX_PINS[tx], LOW);
    delay(5);
  }
}

void kopfzeileDrucken() {
  Serial.println();
  Serial.println(F("=========================================="));
  Serial.println(F("   AMIR ETHERNET DURCHGANGSPRUEFER"));
  Serial.println(F("   Autor : Amir Mobasheraghdam"));
  Serial.println(F("   Thema : Orange / Weiss"));
  Serial.println(F("=========================================="));
}

void kanalberichtDrucken(int kanal) {
  Serial.print(F("Ader "));
  Serial.print(kanal + 1);
  Serial.print(F(": "));

  if (ergebnis.kurzschluss[kanal]) {
    Serial.println(F("KURZSCHLUSS"));
    return;
  }

  if (!ergebnis.verbindung[kanal]) {
    Serial.println(F("UNTERBROCHEN"));
    return;
  }

  if (ergebnis.falschVerdrahtet[kanal]) {
    Serial.print(F("FALSCH VERDRAHTET -> erkannt an RX "));
    Serial.println(ergebnis.erkannterPin[kanal] + 1);
    return;
  }

  Serial.print(F("OK -> RX "));
  Serial.println(ergebnis.erkannterPin[kanal] + 1);
}

void zusammenfassungDrucken() {
  kopfzeileDrucken();

  for (int i = 0; i < CHANNEL_COUNT; i++) {
    kanalberichtDrucken(i);
  }

  Serial.println(F("------------------------------------------"));

  if (ergebnis.bestanden) {
    Serial.println(F("ERGEBNIS: BESTANDEN"));
  } else {
    Serial.println(F("ERGEBNIS: FEHLER"));
  }

  Serial.println(F("=========================================="));
}

void setup() {
  Serial.begin(115200);
  pinsEinrichten();
  delay(250);
  kopfzeileDrucken();
}

void loop() {
  kabelTesten();
  zusammenfassungDrucken();
  delay(LOOP_DELAY_MS);
}
