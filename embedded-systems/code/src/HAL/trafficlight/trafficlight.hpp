/**
 * @file trafficlight.hpp
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk, Oleksandr Maksymov
 * @date 20.01.2026
 * @version 1.0
 * @brief Definition der AmpelLED-Klasse zur Steuerung der Ampellichter.
 *
 * Ermöglicht das Ein-, Ausschalten und Blinken der Ampelfarben (Rot, Gelb, Grün)
 * inklusive Threading-Unterstützung für das Blinken.
 */

#ifndef TRAFFICLIGHT_HPP
#define TRAFFICLIGHT_HPP

#include "../../gpio_address.hpp"
#include "color.hpp"
#include "mode.hpp"

// Hinzugefügt für Threading und IO
#include <thread>
#include <atomic>
#include <sys/mman.h>
#include <sys/neutrino.h>
#include <hw/inout.h>
#include <unistd.h>

class AmpelLED {
private:
    Farbe farbeLED;
    Modus modusLED;

    // --- Thread-Verwaltung (Hinzugefügt) ---
    std::thread blinkThread; // Der Thread für das Blinken
    std::atomic<bool> stopThreadFlag; // Flag zum sicheren Stoppen

    /**
     * Interne Methode: Stoppt den laufenden Blink-Thread sicher.
     */
    void stopBlinken();

    /**
     * Interne Methode: Die Logik, die im Thread ausgeführt wird.
     */
    void runBlinkThread(Farbe farbe, int delay_ms);

    void setModus(Modus given_modus);

    void setFarbe(Farbe given_farbe);

public:
    AmpelLED();

    virtual ~AmpelLED(); // Destruktor hinzugefügt

    Modus getModus();

    Farbe getFarbe();

    bool istModusLeuchtent();

    // Funktioniert blockierend (usleep)
    void beleuchteFuerMS(Farbe given_farbe, useconds_t dauer_s);

    // Startet einen Hintergrund-Thread (Nicht blockierend)
    void blinkenAn(Farbe given_farbe, Modus given_modus);

    // Stoppt das Blinken und schaltet aus
    void blinkenAus(Farbe given_farbe);

    // Schaltet dauerhaft an
    void lichtAn(Farbe given_farbe);

    // Schaltet aus
    void lichtAus(Farbe given_farbe);
};

#endif