# 🔶 kabeldiagnose-system

Ein professionelles und modernes System zur Kabelprüfung und Fehlerdiagnose auf Basis von Arduino.

👨‍💻 Entwickelt von **Amir Mobasheraghdam**

---

## 🚀 Überblick

**kabeldiagnose-system** ist ein Open-Source-Projekt zur präzisen Analyse und Prüfung von mehradrigen Kabeln, insbesondere Ethernet (RJ45).

Das System erkennt automatisch:

* 🔌 Durchgang (Continuity)
* 🔀 Falsche Verdrahtung (Miswire)
* ⚠️ Kurzschluss (Short Circuit)
* ❌ Unterbrechung (Open Line)

Dieses Projekt wurde vollständig neu entwickelt mit Fokus auf **Zuverlässigkeit, Erweiterbarkeit und Performance**.

---

## ⚙️ Funktionen

* ✅ 8-Kanal Kabeldiagnose
* ✅ Echtzeit-Ausgabe über Serial Monitor
* ✅ Erweiterbare Pin-Konfiguration
* ✅ Strukturierter und sauberer Arduino-Code
* ✅ Modernes Orange/Weiß Design
* ✅ GitHub-optimierte Projektstruktur
* ✅ PlatformIO kompatibel

---

## 🧰 Hardware Anforderungen

* Arduino Uno / Nano (oder kompatibel)
* RJ45 Breakout / Testadapter
* Jumper Kabel
* Optional: LEDs zur Statusanzeige

---

## 🔌 Pin-Konfiguration

### TX (Ausgänge)

```cpp id="txde01"
2, 3, 4, 5, 6, 7, 8, 9
```

### RX (Eingänge)

```cpp id="rxde01"
10, 11, 12, 13, A0, A1, A2, A3
```

---

## 📦 Installation

### Arduino IDE

1. Projekt öffnen
2. Code hochladen
3. Serial Monitor auf **115200 Baud** setzen

### PlatformIO

```bash id="piode01"
pio run
pio run --target upload
pio device monitor --baud 115200
```

---

## 📁 Projektstruktur

```id="structde01"
kabeldiagnose-system/
├── src/
├── include/
├── docs/
├── assets/
├── examples/
├── metadata.json
├── platformio.ini
├── wiring.md
└── README.md
```

---

## 🧠 Funktionsweise

1. Jeder Ausgangspin wird einzeln aktiviert
2. Eingänge werden gleichzeitig überwacht
3. System analysiert die Signalpfade
4. Fehler werden automatisch erkannt

---

## 📊 Statusanzeige

* 🟧 PASS → Verbindung korrekt
* ❌ OPEN → Leitung unterbrochen
* ⚠️ SHORT → Kurzschluss erkannt
* 🔀 MISWIRE → falsche Verbindung

---

## 🎨 Design

* Orange / Weiß Industrial Theme
* Klar strukturiert und übersichtlich
* Optimiert für schnelle Diagnose

---

## ⚠️ Sicherheitshinweise

❗ Nur für Niederspannung geeignet
❗ Nicht für Hochspannung verwenden

---

## 🔧 Erweiterungen

* 📟 OLED Display Integration
* 📊 Erweiterte Visualisierung
* 📁 Logging-System
* 🔌 Unterstützung für größere Kabel

---

## 📜 Lizenz

MIT License

---

## 🙌 Credits

Inspiriert von Open-Source Kabeltester-Projekten
Neu entwickelt und optimiert von:

👉 **Amir Mobasheraghdam**
