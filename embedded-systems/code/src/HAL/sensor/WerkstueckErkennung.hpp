/**
 * @file WerkstueckErkennung.hpp
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk, Oleksandr Maksymov
 * @date 20.01.2026
 * @version 1.0
 * @brief Header-Datei mit Definition für die Klasse "WerkstueckErkennung".
 * Diese Klasse übernimmt die Erkennung von Werkstücken basierend auf
 * gemessenen Höhenwerten vom Sensor.
 *
 * Die Erkennung erfolgt über:
 *  - Mittelwertberechnung (Durchschnittshöhe)
 *  - Standardabweichung (erkennt z.B. Bohrungen)
 *  - Vergleich mit Referenztypen und deren Toleranzen
 *
 * @author: Alaa Al-Sowmali und Marta Muñoz Merino
 * @version: 1.0
 * @date: 10. Dec 2025
 */

#ifndef WERKSTUECKERKENNUNG_HPP
#define WERKSTUECKERKENNUNG_HPP

#include <vector>
#include <string>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <iomanip>

#define ANFANG_OFFSET 0.10
#define ENDE_OFFSET 0.95


/**
 * Klasse zur Erkennung unterschiedlicher Werkstücktypen anhand gemessener Höhenwerte.
 * Die Werte kommen typischerweise vom Höhenmesssensor.
 */
class WerkstueckErkennung {
private:

	float hoeheOhneStueck;
	float toleranz;

    /**
     * Sammeln der Messwerte eines einzelnen Werkstücks.
     * Diese Liste wird gefüllt, bis der Sensor einen "Abfall" erkennt.
     */
    std::vector<float> currentValues;

    /**
     * Gibt an, ob aktuell ein Werkstück gemessen wird.
     * true  = Werte sammeln
     * false = kein Werkstück / auf Trigger warten
     */
    bool measuring;

    /**
     * Schwellwert zur Erkennung, wann ein Werkstück beginnt.
     * Beispiel: Wenn die Höhe < threshold → Start Messung.
     */
    float threshold;

    int holeCounter; //Fuer die Erkennung von WS Hoch mit Bohrung

    int MAX_HOLE_SIZE; // Fuer die Erkennung von WS Hoch mit Bohrung


    /**
     * Berechnet den Mittelwert (average height) eines Datensatzes.
     * data: Vektor der Werte
     * Rückgabe: Mittelwert
     */
    float calculateMean(const std::vector<float>& data);

    /**
     * Berechnet die Standardabweichung eines Datensatzes.
     * Wichtiger Indikator: Bohrungen → größere Schwankungen → höhere Std.
     * data: Vektor der Werte
     * Rückgabe: Standardabweichung
     */
    float calculateStd(const std::vector<float>& data);

    /**
     * Vergleicht den gemessenen Mittelwert und die Standardabweichung
     * mit den Referenztypen der Werkstücke.
     *
     * Gibt den Namen des erkannten Werkstücks zurück.
     * values: Vektor aller gemessenen Werte
     * Rückgabe: Name des Werkstücktyps
     */
    std::string recognizeWerkstueck(const std::vector<float>& values);

public:

    /**
     * Standardkonstruktor mit Default-Schwellenwert.
     */
    WerkstueckErkennung() : hoeheOhneStueck(0), toleranz(0), measuring(false), threshold(18), holeCounter(0), MAX_HOLE_SIZE(40000) {}

    /**
     * Konstruktor mit individuellem Threshold.
     */
    WerkstueckErkennung(float thr);

	//Getter für den Schwellwert
	float getThreshold() const {
		return threshold;
	}
    /**
     * Wird für jeden neuen Sensorwert aufgerufen.
     * Steuert Messbeginn, Messende und Erkennung.
     *
     * value: aktueller Sensorwert (Höhe)
     * Return:
     *  - "" (leer) solange kein Werkstück fertig gemessen wurde
     *  - Name des erkannten Werkstücks bei Abschluss der Messung
     */
    std::string processValue(float value);


    void kalibrieren(float wert);


    // --- Werkstuecke Definition ---
        struct WerkstueckTyp {
            std::string label; // Typ in String
            float height;      // Max hoehe
            float std;         // Abweichung
            int holes;			// Bohrung
        };

        /**
         * DATA VON WERKSTUECK
         */
        const std::vector<WerkstueckTyp> dataset = {
                // --- FLACH ---

                { "Flach ohne Bohrung",  7.0f, 0.10f, 0 },
                { "Flach ohne Bohrung",  7.5f, 0.20f, 0 },
                { "Flach ohne Bohrung",  7.8f, 0.02f, 0 },

                // --- HOCH   ---
                { "Hoch ohne Bohrung",   9.0f, 0.01f, 0 },
                { "Hoch ohne Bohrung",   9.0f, 0.00f, 0 },
				{ "Hoch ohne Bohrung",   8.0f, 0.00f, 0 },


                // --- HOCH MIT BOHRUNG  ---
				{ "Hoch mit Bohrung",    9.0f, 0.01f, 1 },
                { "Hoch mit Bohrung",    9.0f, 0.52f, 1 },
                { "Hoch mit Bohrung",    9.0f, 0.00f, 1 },
				{ "Hoch mit Bohrung",    9.0f, 2.90f, 3 },
				{ "Hoch mit Bohrung",    9.0f, 1.35f, 1 },
				{ "Hoch mit Bohrung",    9.2f, 1.35f, 1 },
				{ "Hoch mit Bohrung",    8.9f, 2.90f, 1 },
				{ "Hoch mit Bohrung",    8.8f, 0.00f, 1 },
				//{ "Hoch mit Bohrung",    9.0f, 0.02f, 0 }, damit 10 gut lauft
				//{ "Hoch mit Bohrung",    8.0f, 0.42f, 0 }, damit anlage 10 gut lauft
				{ "Hoch mit Bohrung",    8.6f, 0.42f, 0 },
				{ "Hoch mit Bohrung",    8.3f, 2.10f, 1 },
				// Agujero profundo

                // --- BINAERE ---
                { "Unbekannt / Sonderfall", 8.5f, 0.74f, 2 },
                { "Unbekannt / Sonderfall", 9.2f, 0.80f, 2 },
				{ "Unbekannt / Sonderfall", 9.2f, 0.80f, 2 },
				{ "Unbekannt / Sonderfall", 9.0f, 0.80f, 2 },
				{ "Unbekannt / Sonderfall", 9.0f, 0.48f, 0 },
				{ "Unbekannt / Sonderfall", 9.7f, 0.48f, 2},
				{ "Unbekannt / Sonderfall", 9.1f, 0.80f, 1},
				{ "Unbekannt / Sonderfall", 8.5f, 0.50f, 0},
            };
};

#endif
