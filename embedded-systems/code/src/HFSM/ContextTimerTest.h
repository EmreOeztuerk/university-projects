/**
* @file ContextTimerTest.h
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk
 * @date 20.01.2026
 * @version 1.0
 * @brief Hilfsklasse für Timer-Tests im Kontext.
 *
 * Timer Test
 */

#ifndef CONTEXTTIMERTEST_H
#define CONTEXTTIMERTEST_H

#include <iostream>
#include <chrono>
#include <iomanip>
#include "ContextData.h"
#include "Actions.h"
#include "../Dispatcher/signal.hpp"
#include "regionTimer.hpp"

class ContextTimerTest {
private:
    ContextData *data;
    Actions *action;

    int testSchritt = 0;
    bool motorLaeuftSchnell = true;
    bool motorLaeuft = false;
    std::chrono::steady_clock::time_point lastTime;

    double wegTimer_ms = 0.0;
    //double t_s = 3548;
    //double t_l = 10474;
    bool lsaGesperrt = false;
    bool lshGesperrt = false;
    bool startGesperrt = false;

public:
    ContextTimerTest(ContextData *d, Actions *a) : data(d), action(a) {
        lastTime = std::chrono::steady_clock::now();
    }

    void tick() {
        auto now = std::chrono::steady_clock::now();
        double delta_ms = std::chrono::duration<double, std::milli>(now - lastTime).count();
        lastTime = now;

        if (motorLaeuft) {
            // RegionTimer aktualisieren
            data->lsa_timer.aktualisiere(delta_ms, motorLaeuftSchnell);

            if (testSchritt == 2) {
                wegTimer_ms += delta_ms;

                // Kalibrierte Werte abrufen
                double t_schnell = data->holeZeit("lsa_lsh_s");
                double t_langsam = data->holeZeit("lsa_lsh_l");

                // Automatischer Wechsel: LANGSAM -> SCHNELL -> LANGSAM
                if (wegTimer_ms < 1500.0) {
                    if (motorLaeuftSchnell || wegTimer_ms < delta_ms) {
                        std::cout << "[Ueberwachung] Phase 1: LANGSAM | Kalibriert (l): " << t_langsam << " ms" <<
                                std::endl;
                        motorLaeuftSchnell = false;
                        action->cmd_mtr_rechts_l();
                    }
                } else if (wegTimer_ms >= 1500.0 && wegTimer_ms < 3000.0) {
                    if (!motorLaeuftSchnell) {
                        std::cout << "[Ueberwachung] Phase 2: SCHNELL | Kalibriert (s): " << t_schnell << " ms" <<
                                std::endl;
                        motorLaeuftSchnell = true;
                        action->cmd_mtr_rechts_s();
                    }
                } else if (wegTimer_ms >= 3000.0) {
                    if (motorLaeuftSchnell) {
                        std::cout << "[Ueberwachung] Phase 3: LANGSAM | Kalibriert (l): " << t_langsam << " ms" <<
                                std::endl;
                        motorLaeuftSchnell = false;
                        action->cmd_mtr_rechts_l();
                    }
                }
            }
        }
    }

    void handleHardwareSignal(Signal sig) {
        switch (sig) {
            case Signal::S_ST1_G:
                if (!startGesperrt) {
                    startGesperrt = true;
                    motorLaeuft = true;
                    if (testSchritt == 0) {
                        std::cout << "\n[Test] === SCHRITT 0: SCHNELL START ===" << std::endl;
                        motorLaeuftSchnell = true;
                        action->cmd_mtr_rechts_s();
                    } else if (testSchritt == 1) {
                        std::cout << "\n[Test] === SCHRITT 1: LANGSAM START ===" << std::endl;
                        motorLaeuftSchnell = false;
                        action->cmd_mtr_rechts_l();
                    } else if (testSchritt == 2) {
                        std::cout << "\n[Test] === SCHRITT 2: UEBERWACHUNG START ===" << std::endl;
                        wegTimer_ms = 0.0;
                        motorLaeuftSchnell = true; // Startet laut Logik in tick() langsam
                        action->cmd_mtr_rechts_s();
                    }
                }
                break;

            case Signal::S_LSA1_UB:
                if (!lsaGesperrt) {
                    lsaGesperrt = true;
                    std::cout << "[Test] LSA erreicht - zeitStarten()" << std::endl;
                    data->zeitStarten();

                    if (testSchritt == 2) {
                        double t_s = data->holeZeit("lsa_lsh_s");
                        double t_l = data->holeZeit("lsa_lsh_l");
                        data->lsa_timer.starte(t_s, t_l,
                                               []() {
                                                   std::cout << "-> TIMER: Basiszeit (100% Weg) erreicht." << std::endl;
                                               },
                                               []() {
                                                   std::cout <<
                                                           "-> FEHLER: WS verloren oder Toleranzzeit ueberschritten!" <<
                                                           std::endl;
                                               });
                    }
                }
                break;

            case Signal::S_LSA1_NUB:
                lsaGesperrt = false;
                startGesperrt = false;
                break;

            case Signal::S_LSW_UB:
                if (!lshGesperrt) {
                    lshGesperrt = true;
                    std::cout << "[Test] LSH erreicht - Motor Stop" << std::endl;
                    action->cmd_mtr_stop();
                    motorLaeuft = false;

                    double gemesseneZeit = 0.0;
                    if (testSchritt == 0) {
                        data->zeitSpeichern(gemesseneZeit, "lsa_lsh_s");
                        std::cout << "[Check] Zeit SCHNELL gespeichert: " << gemesseneZeit << " ms" << std::endl;
                        testSchritt = 1;
                    } else if (testSchritt == 1) {
                        data->zeitSpeichern(gemesseneZeit, "lsa_lsh_l");
                        std::cout << "[Check] Zeit LANGSAM gespeichert: " << gemesseneZeit << " ms" << std::endl;
                        testSchritt = 2;
                    } else {
                        std::cout << "[Test] Ueberwachung abgeschlossen." << std::endl;
                    }
                }
                break;

            case Signal::S_LSW_NUB:
                lshGesperrt = false;
                break;

            default: break;
        }
    }
};
#endif