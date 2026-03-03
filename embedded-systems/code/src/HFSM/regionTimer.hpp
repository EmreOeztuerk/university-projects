/**
 * @file regionTimer.hpp
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk
 * @date 20.01.2026
 * @version 1.0
 * @brief Timer-Klasse für FSM-Regionen.
 *
 * Ein Software-Timer, der innerhalb bestimmter Zustände oder Regionen verwendet wird,
 * um Zeitüberschreitungen zu messen.
 */

#include <iostream>
#include "../Dispatcher/dispatcher.hpp"
#include "actions.h"

class RegionTimer {
private:
    Actions *action;
    // Funktions-Pointer für die verschiedenen Events
    std::function<void()> onZeitAbgelaufen;
    std::function<void()> onToleranzAbgelaufen;


    double toleranzZaehler = 0.0; // Separater Zähler für die Toleranzphase
    double fortschritt = 0.0; // Der Weg-Fortschritt (0.0 bis 1.0)
    double t_schnell = 0.0; // Kalibrierter Wert aus Messung
    double t_langsam = 0.0; // Kalibrierter Wert aus Messung

    bool istAktiv = false;
    bool istPausiert = false;
    bool istInToleranzPhase = false;
    bool geschwindigkeitSchnell = true;

public:
    // Konstruktor nimmt die Actions entgegen
    RegionTimer(Actions *action) : action(action), istAktiv(false) {
    }

    // Erweitere starte() um die Funktionsaufrufe
    void starte(double schnell_ms, double langsam_ms,
                std::function<void()> zeitAction,
                std::function<void()> toleranzAction) {
        this->t_schnell = schnell_ms;
        this->t_langsam = langsam_ms;
        this->onZeitAbgelaufen = zeitAction;
        this->onToleranzAbgelaufen = toleranzAction;
        this->fortschritt = 0.0;
        this->toleranzZaehler = 0.0;
        this->istAktiv = true;
        this->istPausiert = false;
        this->istInToleranzPhase = false;
        std::cout << "[RegionTimer] Ueberwachung wird gestartet" << std::endl;
    }

    void aktualisiere(double delta_ms, bool istSchnell) {
        // Wenn nicht aktiv oder pausiert -> nichts tun, Werte bleiben erhalten
        if (!istAktiv || istPausiert) return;

        this->geschwindigkeitSchnell = istSchnell;

        if (!istInToleranzPhase) {
            double basisZeit = geschwindigkeitSchnell ? t_schnell : t_langsam;

            // Berechnung des Fortschrittsanteils
            if (basisZeit > 0) {
                fortschritt += delta_ms / basisZeit;
            }
            // Prüfung, ob 100% erreicht sind
            if (fortschritt >= 0.93) {
                fortschritt = 1.0;
                istInToleranzPhase = true;
                if (onZeitAbgelaufen) {
                    onZeitAbgelaufen();
                }
            }
        } else {
            // Toleranzphase: 20% der aktuellen Basiszeit
            double basisZeit = geschwindigkeitSchnell ? t_schnell : t_langsam;
            double toleranzLimit = basisZeit * 0.4;
            toleranzZaehler += delta_ms;

            if (toleranzZaehler >= toleranzLimit) {
                istAktiv = false;
                if (onToleranzAbgelaufen) onToleranzAbgelaufen();
            }
        }
    }

    void pausiere() {
        if (istAktiv) {
            istPausiert = true;
            std::cout << "[RegionTimer] Überwachung pausiert (Eingefroren)" << std::endl;
        }
    }

    void fortsetzen() {
        if (istAktiv) {
            istPausiert = false;
            std::cout << "[RegionTimer] Überwachung fortgesetzt" << std::endl;
        }
    }

    void stoppeVollstaendig() {
        istAktiv = false;
        istPausiert = false;
        std::cout << "[RegionTimer] stoppeVollstaendig " << std::endl;
    }
};