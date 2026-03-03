/**
 * @file trafficlight.cpp
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk, Oleksandr Maksymov
 * @date 20.01.2026
 * @version 1.0
 * @brief Implementierung der AmpelLED-Klasse.
 *
 * Verwaltet die Ansteuerung der Ampellichter inklusive eines Hintergrund-Threads
 * für das Blinken der LEDs, ohne den Hauptablauf zu blockieren.
 */

#include "trafficlight.hpp"
#include <iostream>
#include <chrono> // Für std::chrono in threads

// ==========================================
// KONSTRUKTOR / DESTRUKTOR
// ==========================================

AmpelLED::AmpelLED() {
    farbeLED = Farbe::LED_GELB;
    modusLED = Modus::LED_AUS;
    stopThreadFlag = false; // Initialisieren
}

AmpelLED::~AmpelLED() {
    // WICHTIG: Beim Zerstören des Objekts muss der Thread beendet werden.
    stopBlinken();
}

// ==========================================
// PRIVATE HILFSMETHODEN
// ==========================================

void AmpelLED::stopBlinken() {
    // 1. Flag setzen, damit die Schleife im Thread endet
    stopThreadFlag = true;

    // 2. Warten, bis der Thread fertig ist (Join)
    if (blinkThread.joinable()) {
        blinkThread.join();
    }

    // 3. Reset für den nächsten Start
    stopThreadFlag = false;
}

void AmpelLED::runBlinkThread(Farbe given_farbe, int delay_ms) {
    // QNX: Jeder Thread braucht IO-Rechte
    if (ThreadCtl(_NTO_TCTL_IO, 0) == -1) return;

    // Speicher mappen (einmalig für den Thread)
    uintptr_t gpio_bank = mmap_device_io(MAP_SIZE, (uint64_t) GPIO_BANK_1);
    if (gpio_bank == MAP_DEVICE_FAILED) return;

    while (!stopThreadFlag) {
        // AN
        out32((uintptr_t)(gpio_bank + OFFSET_SET_REG), given_farbe);

        // Warten (Sleep) in kleinen Schritten oder direkt
        std::this_thread::sleep_for(std::chrono::milliseconds(delay_ms));
        if (stopThreadFlag) break; // Sofort raus, wenn Stopp angefordert

        // AUS
        out32((uintptr_t)(gpio_bank + OFFSET_CRL_REG), given_farbe);

        // Warten
        std::this_thread::sleep_for(std::chrono::milliseconds(delay_ms));
    }

    // Aufräumen
    munmap_device_io(gpio_bank, MAP_SIZE);
}

// ==========================================
// GETTER / SETTER
// ==========================================

Modus AmpelLED::getModus() {
    return modusLED;
}

Farbe AmpelLED::getFarbe() {
    return farbeLED;
}

void AmpelLED::setModus(Modus given_modus) {
    modusLED = given_modus;
}

void AmpelLED::setFarbe(Farbe given_farbe) {
    farbeLED = given_farbe;
}

bool AmpelLED::istModusLeuchtent() {
    return (getModus() == Modus::LED_AN ||
            getModus() == Modus::BLINK_LANGSAM ||
            getModus() == Modus::BLINK_SCHNELL);
}

// ==========================================
// HAUPTFUNKTIONEN (PUBLIC)
// ==========================================

void AmpelLED::beleuchteFuerMS(Farbe given_farbe, useconds_t dauer_s) {
    // Vorheriges Blinken stoppen, um Konflikte zu vermeiden
    stopBlinken();

    if (ThreadCtl(_NTO_TCTL_IO, 0) == -1) return;

    uintptr_t gpio_bank_1 = mmap_device_io(MAP_SIZE, (uint64_t) GPIO_BANK_1);
    if (gpio_bank_1 == MAP_DEVICE_FAILED) return;

    // Kurz an
    out32((uintptr_t)(gpio_bank_1 + OFFSET_SET_REG), given_farbe);
    usleep(dauer_s); // Blockierend!
    // Aus
    out32((uintptr_t)(gpio_bank_1 + OFFSET_CRL_REG), given_farbe);

    munmap_device_io(gpio_bank_1, MAP_SIZE);
}

void AmpelLED::blinkenAn(Farbe given_farbe, Modus given_modus) {
    // 1. Alles stoppen (Clean Slate)
    stopBlinken();

    // 2. Hardware sicherstellen (LED aus)
    lichtAus(given_farbe);

    // 3. Zustand setzen
    setFarbe(given_farbe);
    setModus(given_modus);

    // 4. Geschwindigkeit festlegen (Beispielwerte in ms)
    /*    int delay_ms = 1000; // Standard Langsam
    if (given_modus == Modus::BLINK_SCHNELL) {
        delay_ms = 500;
    }*/

    int delay_ms = (static_cast<int>(given_modus) * 1000) / 2;

    // 5. Neuen Thread starten
    stopThreadFlag = false;
    // std::thread startet die Funktion runBlinkThread im Hintergrund
    blinkThread = std::thread(&AmpelLED::runBlinkThread, this, given_farbe, delay_ms);
}

void AmpelLED::blinkenAus(Farbe given_farbe) {
    // Einfach lichtAus aufrufen, das erledigt jetzt alles (Thread killen + GPIO aus)
    lichtAus(given_farbe);
}

void AmpelLED::lichtAn(Farbe given_farbe) {
    // 1. Thread stoppen, falls aktiv
    stopBlinken();

    if (ThreadCtl(_NTO_TCTL_IO, 0) == -1) return;

    uintptr_t gpio_bank_1 = mmap_device_io(MAP_SIZE, (uint64_t) GPIO_BANK_1);
    if (gpio_bank_1 != MAP_DEVICE_FAILED) {
        // Anschalten
        out32((uintptr_t)(gpio_bank_1 + OFFSET_SET_REG), given_farbe);
        munmap_device_io(gpio_bank_1, MAP_SIZE);
    }

    setFarbe(given_farbe);
    setModus(Modus::LED_AN);
}

void AmpelLED::lichtAus(Farbe given_farbe) {
    // 1. Thread stoppen (WICHTIG!)
    stopBlinken();

    // 2. Bedingung vereinfacht: Wenn die Farbe stimmt, schalten wir aus.
    // Wir prüfen NICHT auf modusLED, damit wir auch blinkende Lichter ausschalten können.
    if (getFarbe() == given_farbe) {
        if (ThreadCtl(_NTO_TCTL_IO, 0) == -1) return;

        uintptr_t gpio_bank_1 = mmap_device_io(MAP_SIZE, (uint64_t) GPIO_BANK_1);
        if (gpio_bank_1 != MAP_DEVICE_FAILED) {
            // Ausschalten (Clear Register)
            out32((uintptr_t)(gpio_bank_1 + OFFSET_CRL_REG), given_farbe);
            munmap_device_io(gpio_bank_1, MAP_SIZE);
        }

        setFarbe(given_farbe);
        setModus(Modus::LED_AUS);
    }
}