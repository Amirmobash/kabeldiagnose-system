#include "cable_map.h"

struct PruefErgebnis {
  bool hatVerbindung[CHANNEL_COUNT];
  bool istFalschVerdrahtet[CHANNEL_COUNT];
  bool hatKurzschluss[CHANNEL_COUNT];
  int erkannterRxPin[CHANNEL_COUNT];
  bool istBestanden;
};

PruefErgebnis ergebnis;

void pinsVorbereiten() {
  for (int i = 0; i < CHANNEL_COUNT; i++) {
    pinMode(TX_PINS[i], OUTPUT);
    digitalWrite(TX_PINS[i], LOW);
    pinMode(RX_PINS[i], INPUT_PULLUP);
  }
}

void ergebnisLoeschen() {
  ergebnis.istBestanden = true;

  for (int i = 0; i < CHANNEL_COUNT; i++) {
    ergebnis.hatVerbindung[i] = false;
    ergebnis.istFalschVerdrahtet[i] = false;
    ergebnis.hatKurzschluss[i] = false;
    ergebnis.erkannterRxPin[i] = -1;
  }
}

void alleSenderAusschalten() {
  for (int i = 0; i < CHANNEL_COUNT; i++) {
    digitalWrite(TX_PINS[i], LOW);
  }
}

int aktiveEmpfaengerZaehlen(int &ersterTreffer) {
  int anzahlTreffer = 0;
  ersterTreffer = -1;

  for (int i = 0; i < CHANNEL_COUNT; i++) {
    if (digitalRead(RX_PINS[i]) == LOW) {
      if (ersterTreffer == -1) {
        ersterTreffer = i;
      }
      anzahlTreffer++;
    }
  }

  return anzahlTreffer;
}

void einzelneAderPruefen(int sender) {
  alleSenderAusschalten();
  delay(2);

  digitalWrite(TX_PINS[sender], HIGH);
  delay(STABILIZE_DELAY_MS);

  int erkannterPin = -1;
  int treffer = aktiveEmpfaengerZaehlen(erkannterPin);

  if (treffer == 0) {
    ergebnis.hatVerbindung[sender] = false;
    ergebnis.istBestanden = false;
  } else if (treffer > 1) {
    ergebnis.hatVerbindung[sender] = false;
    ergebnis.hatKurzschluss[sender] = true;
    ergebnis.istBestanden = false;
  } else {
    ergebnis.hatVerbindung[sender] = true;
    ergebnis.erkannterRxPin[sender] = erkannterPin;

    if (erkannterPin != EXPECTED_MAP[sender]) {
      ergebnis.istFalschVerdrahtet[sender] = true;
      ergebnis.istBestanden = false;
    }
  }

  digitalWrite(TX_PINS[sender], LOW);
  delay(5);
}

void kabelPruefen() {
  ergebnisLoeschen();

  for (int i = 0; i < CHANNEL_COUNT; i++) {
    einzelneAderPruefen(i);
  }
}

void titelDrucken() {
  Serial.println();
  Serial.println(F("=========================================="));
  Serial.println(F("        ETHERNET KABELTESTER"));
  Serial.println(F("        Autor: Amir Mobasheraghdam"));
  Serial.println(F("        Design: Orange / Weiss"));
  Serial.println(F("=========================================="));
}

void aderBerichtDrucken(int ader) {
  Serial.print(F("Ader "));
  Serial.print(ader + 1);
  Serial.print(F(": "));

  if (ergebnis.hatKurzschluss[ader]) {
    Serial.println(F("Kurzschluss erkannt"));
    return;
  }

  if (!ergebnis.hatVerbindung[ader]) {
    Serial.println(F("Keine Verbindung"));
    return;
  }

  if (ergebnis.istFalschVerdrahtet[ader]) {
    Serial.print(F("Falsch verdrahtet, erkannt an RX "));
    Serial.println(ergebnis.erkannterRxPin[ader] + 1);
    return;
  }

  Serial.print(F("In Ordnung, erkannt an RX "));
  Serial.println(ergebnis.erkannterRxPin[ader] + 1);
}

void ergebnisDrucken() {
  titelDrucken();

  for (int i = 0; i < CHANNEL_COUNT; i++) {
    aderBerichtDrucken(i);
  }

  Serial.println(F("------------------------------------------"));

  if (ergebnis.istBestanden) {
    Serial.println(F("Gesamtergebnis: BESTANDEN"));
  } else {
    Serial.println(F("Gesamtergebnis: FEHLER GEFUNDEN"));
  }

  Serial.println(F("=========================================="));
}

void setup() {
  Serial.begin(115200);
  pinsVorbereiten();
  delay(250);
  titelDrucken();
}

void loop() {
  kabelPruefen();
  ergebnisDrucken();
  delay(LOOP_DELAY_MS);
}
